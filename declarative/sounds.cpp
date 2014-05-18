/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2014 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "sounds.h"
#include <QCoreApplication>
#include <QDBusServiceWatcher>
#include <QDBusConnection>
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif
#include <pulse/pulseaudio.h>
#include <sndfile.h>

#define CAMERA_IMAGE_START_SOUND_ID     "cameraplus-image-start"
#define CAMERA_IMAGE_END_SOUND_ID       "cameraplus-image-end"
#define CAMERA_VIDEO_START_SOUND_ID     "cameraplus-video-start"
#define CAMERA_VIDEO_STOP_SOUND_ID      "cameraplus-video-stop"
#define CAMERA_FOCUS_SUCCESS_SOUND_ID   "cameraplus-focus-success"
#define CAMERA_FOCUS_FAILURE_SOUND_ID   "cameraplus-focus-failure"
#define MEDIA_ROLE                      "camera-sound-effect"

#define FULL_VOLUME         65536
#define HEADSET_VOLUME      26090

// EVERYTHING between #ifdef SAILFISH #else is _A BIG FAT UGLY HACK_
// DO NOT COPY AND PASTE IT. You WILL GET NIGHTMARES.

class FileReader {
public:
  typedef enum {
    StatusOk,
    StatusEof,
    StatusError,
  } Status;

  FileReader(const QString& path);
  ~FileReader();

  Status writeToStream(pa_stream *p, size_t nbytes);
  pa_sample_spec *sampleSpec();
  size_t size();

private:
  void fillSampleSpec();

  pa_sample_spec m_s;
  SNDFILE *m_file;
  SF_INFO m_info;
  size_t m_pos;
};

class SoundFileInfo {
public:
  SoundFileInfo(const QString& path) :
    m_duration(0) {

    reset(path);
  }

  void reset(const QString& path) {
    m_path = path;
    m_duration = 0;
  }

  QString path() const {
    return m_path;
  }

  void setDuration(useconds_t duration) {
    m_duration = duration;
  }

  useconds_t duration() const {
    return m_duration;
  }

private:
  QString m_path;
  useconds_t m_duration;
};

FileReader::FileReader(const QString& path) :
  m_pos(0) {

  memset(&m_s, 0, sizeof(m_s));
  memset(&m_info, 0, sizeof(m_info));
  m_file = sf_open(path.toLocal8Bit().constData(), SFM_READ, &m_info);
  fillSampleSpec();
}

FileReader::~FileReader() {
  if (m_file) {
    sf_close(m_file);
    m_file = 0;
  }
}

FileReader::Status FileReader::writeToStream(pa_stream *p, size_t nbytes) {
  int frameSize = pa_frame_size(&m_s);
  int len = qMin(nbytes, (nbytes/frameSize) * nbytes);
  void *buff = pa_xmalloc(len);
  if (!buff) {
    qWarning() << "Failed to allocate buffer";
    return StatusError;
  }

  sf_count_t read = sf_read_raw(m_file, buff, len);

  if (pa_stream_write(p, buff, read, pa_xfree, 0, PA_SEEK_RELATIVE) < 0) {
    qWarning() << "Failed to write to pulse audio stream";
    pa_xfree(buff);
    return StatusError;
  }

  m_pos += read;
  if (m_pos == size()) {
    // Over.
    return StatusEof;
  }

  if (read != len) {
    return StatusError;
  }

  return StatusOk;
}

void FileReader::fillSampleSpec() {
  m_s.rate = m_info.samplerate;
  m_s.channels = m_info.channels;

  // stolen from pulseaudio :p
  switch (m_info.format & SF_FORMAT_SUBMASK) {
  case SF_FORMAT_PCM_16:
  case SF_FORMAT_PCM_U8:
  case SF_FORMAT_PCM_S8:
    m_s.format = PA_SAMPLE_S16NE;
    break;

  case SF_FORMAT_PCM_24:
    m_s.format = PA_SAMPLE_S24NE;
    break;

  case SF_FORMAT_PCM_32:
    m_s.format = PA_SAMPLE_S32NE;
    break;

  case SF_FORMAT_ULAW:
    m_s.format = PA_SAMPLE_ULAW;
    break;

  case SF_FORMAT_ALAW:
    m_s.format = PA_SAMPLE_ALAW;
    break;

  case SF_FORMAT_FLOAT:
  case SF_FORMAT_DOUBLE:
  default:
    m_s.format = PA_SAMPLE_FLOAT32NE;
    break;
  }
}

pa_sample_spec *FileReader::sampleSpec() {
  return &m_s;
}

size_t FileReader::size() {
  return pa_frame_size(&m_s) * m_info.frames;
}

static void contextStateCallback(pa_context *ctx, pa_threaded_mainloop *loop) {
  Q_UNUSED(ctx);

  pa_threaded_mainloop_signal(loop, 0);
}

static void streamStateCallback(pa_stream *stream, pa_threaded_mainloop *loop) {
  Q_UNUSED(stream);

  pa_threaded_mainloop_signal(loop, 0);
}

static void contextSuccessCallback(pa_context *c, int success, pa_threaded_mainloop *loop) {
  Q_UNUSED(c);
  Q_UNUSED(success);

  pa_threaded_mainloop_signal(loop, 0);
}

static void streamRequestCallback(pa_stream *p, size_t nbytes, FileReader *h) {
  switch (h->writeToStream(p, nbytes)) {
  case FileReader::StatusOk:
    break;
  case FileReader::StatusEof:
    pa_stream_set_write_callback(p, NULL, NULL);
    pa_stream_finish_upload(p);
    break;

  case FileReader::StatusError:
    pa_stream_set_write_callback(p, NULL, NULL);
    pa_stream_disconnect(p);
    break;
  }
}

Sounds::Sounds(QObject *parent) :
  QObject(parent),
  m_muted(false),
  m_ctx(0),
  m_loop(pa_threaded_mainloop_new()),
  m_volume(Sounds::VolumeHigh),
  m_watcher(new QDBusServiceWatcher("org.pulseaudio.Server",
				    QDBusConnection::systemBus(),
				    QDBusServiceWatcher::WatchForOwnerChange)) {

  QObject::connect(m_watcher,
		   SIGNAL(serviceOwnerChanged(const QString&, const QString&, const QString&)),
		   this,
		   SLOT(serviceOwnerChanged(const QString&, const QString&, const QString&)));
}

Sounds::~Sounds() {
  destroy();

  pa_threaded_mainloop_free(m_loop);
  m_loop = 0;

  qDeleteAll(m_files);
}

void Sounds::destroy() {
  pa_threaded_mainloop_stop(m_loop);

  if (m_ctx) {
    pa_context_disconnect(m_ctx);
    pa_context_unref(m_ctx);
    m_ctx = 0;
  }
}

void Sounds::serviceOwnerChanged(const QString& serviceName, const QString& oldOwner,
				 const QString& newOwner) {
  Q_UNUSED(serviceName);
  Q_UNUSED(oldOwner);

  if (newOwner.isEmpty()) {
    destroy();
  }
  else if (!newOwner.isEmpty()) {
    reload();
  }
}

void Sounds::playImageCaptureStartedSound() {
  play(CAMERA_IMAGE_START_SOUND_ID);
}

void Sounds::playImageCaptureEndedSound() {
  play(CAMERA_IMAGE_END_SOUND_ID);
}

void Sounds::playVideoRecordingStartedSound() {
  playAndBlock(CAMERA_VIDEO_START_SOUND_ID);
}

void Sounds::playVideoRecordingEndedSound() {
  play(CAMERA_VIDEO_STOP_SOUND_ID);
}

void Sounds::playAutoFocusAcquiredSound() {
  play(CAMERA_FOCUS_SUCCESS_SOUND_ID);
}

void Sounds::playAutoFocusFailedSound() {
  play(CAMERA_FOCUS_FAILURE_SOUND_ID);
}

bool Sounds::isMuted() const {
  return m_muted;
}

void Sounds::setMuted(bool mute) {
  if (mute != m_muted) {
    m_muted = mute;
    emit muteChanged();
  }
}

pa_volume_t Sounds::playbackVolume() const {
  return m_volume == Sounds::VolumeLow ? HEADSET_VOLUME : FULL_VOLUME;
}

void Sounds::reload() {
  if (m_ctx) {
    destroy();
  }

  pa_proplist *prop = pa_proplist_new();
#ifdef SAILFISH
  pa_proplist_sets(prop, PA_PROP_MEDIA_ROLE, "event");
#else
  pa_proplist_sets(prop, PA_PROP_MEDIA_ROLE, MEDIA_ROLE);
#endif

  pa_proplist_sets(prop, PA_PROP_APPLICATION_NAME,
		   qPrintable(QCoreApplication::instance()->applicationName()));
#ifdef SAILFISH
  pa_proplist_sets(prop, PA_PROP_APPLICATION_PROCESS_BINARY, "ngfd");
#endif

  m_ctx = pa_context_new_with_proplist(pa_threaded_mainloop_get_api(m_loop), NULL, prop);
  pa_proplist_free(prop);

  if (!m_ctx) {
    qmlInfo(this) << "Failed to create pulse audio context";
    return;
  }

  pa_context_set_state_callback(m_ctx, (pa_context_notify_cb_t)contextStateCallback, m_loop);

  if (pa_context_connect(m_ctx, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL) < 0) {
    pa_context_unref(m_ctx);
    m_ctx = 0;

    qmlInfo(this) << "Failed to connect to pulse audio";
    return;
  }

  pa_threaded_mainloop_lock(m_loop);
  if (pa_threaded_mainloop_start(m_loop) < 0) {
    pa_threaded_mainloop_unlock(m_loop);
    pa_context_disconnect(m_ctx);
    pa_context_unref(m_ctx);
    m_ctx = 0;
    qmlInfo(this) << "Failed to start pulse audio loop";
    return;
  }

  while (true) {
    bool out = false;

    switch (pa_context_get_state(m_ctx)) {
    case PA_CONTEXT_UNCONNECTED:
    case PA_CONTEXT_CONNECTING:
    case PA_CONTEXT_AUTHORIZING:
    case PA_CONTEXT_SETTING_NAME:
      pa_threaded_mainloop_wait(m_loop);
      continue;

    case PA_CONTEXT_READY:
      out = true;
      break;

    case PA_CONTEXT_FAILED:
    case PA_CONTEXT_TERMINATED:
      pa_threaded_mainloop_unlock(m_loop);
      pa_context_disconnect(m_ctx);
      pa_context_unref(m_ctx);
      m_ctx = 0;
      qmlInfo(this) << "Failed to connect to pulse audio server";
      return;
    }

    if (out) {
      break;
    }
  }

  pa_context_set_state_callback(m_ctx, NULL, NULL);
  pa_threaded_mainloop_unlock(m_loop);

  for (QHash<QString, SoundFileInfo *>::const_iterator iter = m_files.begin();
	 iter != m_files.end(); iter++) {
    cache(iter.key());
  }
}

void Sounds::cache(const QString& id) {
  SoundFileInfo *info = m_files[id];

  if (!info) {
    return;
  }

  if (info->path().isEmpty()) {
    return;
  }

  if (!m_ctx) {
    return;
  }

  FileReader h(info->path());
  pa_sample_spec *spec = h.sampleSpec();

  if (!spec) {
    qmlInfo(this) << "Failed to get a sample spec";
    return;
  }

  if (!pa_sample_spec_valid(spec)) {
    qmlInfo(this) << "Failed to get a valid sample spec";
    return;
  }

  // First we set the file duration
  info->setDuration(pa_bytes_to_usec(h.size(), spec));

  pa_proplist *prop = pa_proplist_new();
  pa_proplist_sets(prop, PA_PROP_MEDIA_ROLE, "event");
#ifdef SAILFISH
  pa_proplist_sets(prop, PA_PROP_MEDIA_NAME, "camera-event");
#else
  pa_proplist_sets(prop, PA_PROP_MEDIA_NAME, qPrintable(id));
#endif
  pa_proplist_sets(prop, PA_PROP_EVENT_ID, qPrintable(id));
  pa_proplist_sets(prop, PA_PROP_MEDIA_FILENAME, qPrintable(info->path()));

#ifdef SAILFISH
  pa_proplist_sets(prop, PA_PROP_APPLICATION_PROCESS_BINARY, "ngfd");
  pa_stream *stream = pa_stream_new_with_proplist(m_ctx, "camera-event",
						  spec, NULL, prop);
#else
  pa_stream *stream = pa_stream_new_with_proplist(m_ctx, qPrintable(id),
						  spec, NULL, prop);
#endif

  pa_proplist_free(prop);
  if (!stream) {
    qmlInfo(this) << "Failed to create a pulse audio stream";
    return;
  }

  pa_stream_set_state_callback(stream, (pa_stream_notify_cb_t)streamStateCallback, m_loop);
  pa_stream_set_write_callback(stream, (pa_stream_request_cb_t)streamRequestCallback, &h);

  pa_threaded_mainloop_lock(m_loop);
  if (pa_stream_connect_upload(stream, h.size()) < 0) {
    pa_stream_unref(stream);
    pa_threaded_mainloop_unlock(m_loop);
    qmlInfo(this) << "Failed to connect pulse audio stream";
    return;
  }

  while (true) {
    bool out = false;

    switch (pa_stream_get_state(stream)) {
    case PA_STREAM_FAILED:
    case PA_STREAM_TERMINATED:
      qmlInfo(this) << "Failed to connect our stream to pulse audio " << pa_strerror(pa_context_errno(m_ctx));
      pa_stream_disconnect(stream);
      pa_stream_unref(stream);
      pa_threaded_mainloop_unlock(m_loop);
;
      return;

    case PA_STREAM_READY:
      pa_threaded_mainloop_unlock(m_loop);
      out = true;
      break;

    case PA_STREAM_UNCONNECTED:
    case PA_STREAM_CREATING:
      pa_threaded_mainloop_wait(m_loop);
      continue;
    }

    if (out) {
      break;
    }
  }

  pa_stream_unref(stream);
}

void Sounds::play(const char *id) {
  if (isMuted()) {
    qmlInfo(this) << "not playing sounds while muted";
    return;
  }

  if (!m_ctx) {
    qmlInfo(this) << "not connected to pulse audio";
    return;
  }

  pa_operation *o = pa_context_play_sample(m_ctx, id, NULL, playbackVolume(), NULL, NULL);

  if (o) {
    pa_operation_unref(o);
  }
}

void Sounds::playAndBlock(const char *id) {
  if (isMuted()) {
    qmlInfo(this) << "not playing sounds while muted";
    return;
  }

  if (!m_ctx) {
    qmlInfo(this) << "not connected to pulse audio";
    return;
  }

  SoundFileInfo *info = m_files[id];
  if (!info) {
    qmlInfo(this) << "unknown sound id " << id;
    return;
  }

  if (!info->duration()) {
    qmlInfo(this) << "unknown file duration";
    return;
  }

  pa_threaded_mainloop_lock(m_loop);
  pa_operation *o = pa_context_play_sample(m_ctx, id, NULL, playbackVolume(),
					   (pa_context_success_cb_t)contextSuccessCallback,
					   m_loop);

  if (!o) {
    qmlInfo(this) << "failed to play sample " << id;
    return;
  }

  bool sleep = false;

  while (true) {
    bool out = false;

    switch (pa_operation_get_state(o)) {
    case PA_OPERATION_RUNNING:
      out = false;
      break;

    case PA_OPERATION_DONE:
      sleep = true;
      out = true;
      break;

    case PA_OPERATION_CANCELLED:
      sleep = false;
      out = true;
      break;
    }

    if (out) {
      break;
    }

    pa_threaded_mainloop_wait(m_loop);
  }

  pa_threaded_mainloop_unlock(m_loop);

  pa_operation_unref(o);

  // Sleep for the duration of the file:
  if (sleep) {
    usleep(info->duration());
  }
}

QString Sounds::imageCaptureStart() const {
  return file(CAMERA_IMAGE_START_SOUND_ID);
}

void Sounds::setImageCaptureStart(const QString& path) {
  if (setFile(path, CAMERA_IMAGE_START_SOUND_ID)) {
    cache(CAMERA_IMAGE_START_SOUND_ID);
    emit imageCaptureStartChanged();
  }
}

QString Sounds::imageCaptureEnd() const {
  return file(CAMERA_IMAGE_END_SOUND_ID);
}

void Sounds::setImageCaptureEnd(const QString& path) {
  if (setFile(path, CAMERA_IMAGE_END_SOUND_ID)) {
    cache(CAMERA_IMAGE_END_SOUND_ID);
    emit imageCaptureEndChanged();
  }
}

QString Sounds::videoRecordingStart() const {
  return file(CAMERA_VIDEO_START_SOUND_ID);
}

void Sounds::setVideoRecordingStart(const QString& path) {
  if (setFile(path, CAMERA_VIDEO_START_SOUND_ID)) {
    cache(CAMERA_VIDEO_START_SOUND_ID);
    emit videoRecordingStartChanged();
  }
}

QString Sounds::videoRecordingEnd() const {
  return file(CAMERA_VIDEO_STOP_SOUND_ID);
}

void Sounds::setVideoRecordingEnd(const QString& path) {
  if (setFile(path, CAMERA_VIDEO_STOP_SOUND_ID)) {
    cache(CAMERA_VIDEO_STOP_SOUND_ID);
    emit videoRecordingEndChanged();
  }
}

QString Sounds::autoFocusAcquired() const {
  return file(CAMERA_FOCUS_SUCCESS_SOUND_ID);
}

void Sounds::setAutoFocusAcquired(const QString& path) {
  if (setFile(path, CAMERA_FOCUS_SUCCESS_SOUND_ID)) {
    cache(CAMERA_FOCUS_SUCCESS_SOUND_ID);
    emit autoFocusAcquiredChanged();
  }
}

QString Sounds::autoFocusFailed() const {
  return file(CAMERA_FOCUS_FAILURE_SOUND_ID);
}

void Sounds::setAutoFocusFailed(const QString& path) {
  if (setFile(path, CAMERA_FOCUS_FAILURE_SOUND_ID)) {
    cache(CAMERA_FOCUS_FAILURE_SOUND_ID);
    emit autoFocusFailedChanged();
  }
}

Sounds::Volume Sounds::volume() const {
  return m_volume;
}

void Sounds::setVolume(const Sounds::Volume& volume) {
  if (m_volume != volume) {
    m_volume = volume;
    emit volumeChanged();
  }
}

bool Sounds::setFile(const QString& path, const QString& id) {
  SoundFileInfo *info = m_files[id];

  if (info && info->path() == path) {
    return false;
  }

  if (!info) {
    info = new SoundFileInfo(path);
    m_files[id] = info;
  } else {
    info->reset(path);
  }

  return true;
}

QString Sounds::file(const QString& id) const {
  SoundFileInfo *info = m_files[id];

  return info ? info->path() : QString();
}
