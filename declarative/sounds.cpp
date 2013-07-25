/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
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
#include <QDebug>
#include "qtcamconfig.h"
#include <QMutex>
#include <QWaitCondition>
#include <QDBusServiceWatcher>
#include <QDBusConnection>
#include <QDeclarativeInfo>

#define CAMERA_IMAGE_START_SOUND_ID  "camera-image-start"
#define CAMERA_IMAGE_END_SOUND_ID    "camera-image-end"
#define CAMERA_VIDEO_START_SOUND_ID  "camera-video-start"
#define CAMERA_VIDEO_STOP_SOUND_ID   "camera-video-stop"
#define CAMERA_FOCUS_SOUND_ID        "camera-focus"

// Odd, volume has to be a char *
#define CANBERRA_FULL_VOLUME         "0.0"
#define CANBERRA_HEADSET_VOLUME      "-24.0"

Sounds::Sounds(QObject *parent) :
  QObject(parent),
  m_muted(false),
  m_ctx(0),
  m_volume(Sounds::VolumeHigh),
  m_watcher(new QDBusServiceWatcher("org.pulseaudio.Server",
				    QDBusConnection::systemBus(),
				    QDBusServiceWatcher::WatchForOwnerChange)) {

  QObject::connect(m_watcher,
		   SIGNAL(serviceOwnerChanged(const QString&, const QString&, const QString&)),
		   this,
		   SLOT(serviceOwnerChanged(const QString&, const QString&, const QString&)));

  // No idea why but canberra will not cache without that!!!
  setenv("CANBERRA_EVENT_LOOKUP", "1", 1);
}

Sounds::~Sounds() {
  if (m_ctx) {
    ca_context_destroy(m_ctx);
    m_ctx = 0;
  }
}

void Sounds::serviceOwnerChanged(const QString& serviceName, const QString& oldOwner,
				 const QString& newOwner) {
  Q_UNUSED(serviceName);
  Q_UNUSED(oldOwner);

  if (newOwner.isEmpty()) {
    // pulse died:
    if (m_ctx) {
      ca_context_destroy(m_ctx);
      m_ctx = 0;
    }
  }
  else if (!newOwner.isEmpty()) {
    reload();
  }
}

void Sounds::playImageCaptureStartedSound() {
  if (isMuted() || !m_ctx) {
    return;
  }

  play(CAMERA_IMAGE_START_SOUND_ID);
}

void Sounds::playImageCaptureEndedSound() {
  if (isMuted() || !m_ctx) {
    return;
  }

  play(CAMERA_IMAGE_END_SOUND_ID);
}

void Sounds::playVideoRecordingStartedSound() {
  if (isMuted() || !m_ctx) {
    return;
  }

  playAndBlock(CAMERA_VIDEO_START_SOUND_ID);
}

void Sounds::playVideoRecordingEndedSound() {
  if (isMuted() || !m_ctx) {
    return;
  }

  play(CAMERA_VIDEO_STOP_SOUND_ID);
}

void Sounds::playAutoFocusAcquiredSound() {
  if (isMuted() || !m_ctx) {
    return;
  }

  play(CAMERA_FOCUS_SOUND_ID);
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

void Sounds::reload() {
  if (m_ctx) {
    ca_context_destroy(m_ctx);
    m_ctx = 0;
  }

  int code = CA_SUCCESS;

  code = ca_context_create(&m_ctx);
  if (code != CA_SUCCESS) {
    qWarning() << "Failed to create canberra context" << ca_strerror(code) << code;
    return;
  }

  code = ca_context_set_driver(m_ctx, "pulse");
  if (code != CA_SUCCESS) {
    qWarning() << "Failed to set canberra driver to pulse audio" << ca_strerror(code) << code;
  }

  code = ca_context_change_props(m_ctx,
				 CA_PROP_MEDIA_ROLE, "camera-sound-effect",
				 NULL);
  if (code != CA_SUCCESS) {
    qWarning() << "Failed to set context properties" << ca_strerror(code) << code;
  }

  code = ca_context_open(m_ctx);
  if (code != CA_SUCCESS) {
    qWarning() << "Failed to open canberra context" << ca_strerror(code) << code;
    ca_context_destroy(m_ctx);
    m_ctx = 0;
    return;
  }

  cache(m_imageCaptureStart, CAMERA_IMAGE_START_SOUND_ID);
  cache(m_imageCaptureEnd, CAMERA_IMAGE_END_SOUND_ID);
  cache(m_videoRecordingStart, CAMERA_VIDEO_START_SOUND_ID);
  cache(m_videoRecordingEnd, CAMERA_VIDEO_STOP_SOUND_ID);
  cache(m_autoFocusAcquired, CAMERA_FOCUS_SOUND_ID);
}

void Sounds::cache(const QString& path, const char *id) {
  if (path.isEmpty()) {
    return;
  }

  int code = ca_context_cache(m_ctx,
			      CA_PROP_EVENT_ID, id,
			      CA_PROP_MEDIA_FILENAME, path.toLocal8Bit().data(),
			      CA_PROP_CANBERRA_CACHE_CONTROL, "permanent",
			      NULL);
  if (code != CA_SUCCESS) {
    qWarning() << "Failed to cache" << path << ca_strerror(code) << code;
  }
}

void Sounds::play(const char *id) {
  if (!m_ctx) {
    qWarning() << "Not connected to pulse audio";
    return;
  }

  const char *volume = m_volume == Sounds::VolumeLow ?
    CANBERRA_HEADSET_VOLUME : CANBERRA_FULL_VOLUME;

  int code = ca_context_play(m_ctx, 0,
			     CA_PROP_CANBERRA_VOLUME, volume,
			     CA_PROP_EVENT_ID, id,
			     CA_PROP_MEDIA_ROLE, "camera-sound-effect",
			     NULL);
  if (code != CA_SUCCESS) {
    qmlInfo(this) << "Failed to play sound" << ca_strerror(code) << code;
  }
}

void ca_finish_callback(ca_context *c, uint32_t id, int error_code, void *userdata) {
  Q_UNUSED(c);
  Q_UNUSED(id);
  Q_UNUSED(error_code);

  QPair<QMutex *, QWaitCondition *> *data =
    static_cast<QPair<QMutex *, QWaitCondition *> *>(userdata);

  data->second->wakeAll();
}

void Sounds::playAndBlock(const char *id) {
  QMutex mutex;
  QWaitCondition cond;
  ca_proplist *p = 0;
  if (ca_proplist_create(&p) != CA_SUCCESS) {
    qDebug() << "Failed to create proplist";
    return;
  }

  ca_proplist_sets(p, CA_PROP_CANBERRA_VOLUME, CANBERRA_FULL_VOLUME);
  ca_proplist_sets(p, CA_PROP_EVENT_ID, id);
  ca_proplist_sets(p, CA_PROP_MEDIA_ROLE, "camera-sound-effect");

  QPair<QMutex *, QWaitCondition *> data = qMakePair<QMutex *, QWaitCondition *>(&mutex, &cond);

  mutex.lock();

  int code = ca_context_play_full(m_ctx, 0, p, ca_finish_callback, &data);

  if (code != CA_SUCCESS) {
    qDebug() << "Failed to play sound" << ca_strerror(code) << code;
    mutex.unlock();
    ca_proplist_destroy(p);

    return;
  }

  cond.wait(&mutex);
  ca_proplist_destroy(p);
  mutex.unlock();
}

QString Sounds::imageCaptureStart() const {
  return m_imageCaptureStart;
}

void Sounds::setImageCaptureStart(const QString& path) {
  if (path != m_imageCaptureStart) {
    m_imageCaptureStart = path;
    cache(m_imageCaptureStart, CAMERA_IMAGE_START_SOUND_ID);
    emit imageCaptureStartChanged();
  }
}

QString Sounds::imageCaptureEnd() const {
  return m_imageCaptureEnd;
}

void Sounds::setImageCaptureEnd(const QString& path) {
  if (path != m_imageCaptureEnd) {
    m_imageCaptureEnd = path;
    cache(m_imageCaptureEnd, CAMERA_IMAGE_END_SOUND_ID);
    emit imageCaptureEndChanged();
  }
}

QString Sounds::videoRecordingStart() const {
  return m_videoRecordingStart;
}

void Sounds::setVideoRecordingStart(const QString& path) {
  if (path != m_videoRecordingStart) {
    m_videoRecordingStart = path;
    cache(m_videoRecordingStart, CAMERA_VIDEO_START_SOUND_ID);
    emit videoRecordingStartChanged();
  }
}

QString Sounds::videoRecordingEnd() const {
  return m_videoRecordingEnd;
}

void Sounds::setVideoRecordingEnd(const QString& path) {
  if (path != m_videoRecordingEnd) {
    m_videoRecordingEnd = path;
    cache(m_videoRecordingEnd, CAMERA_VIDEO_STOP_SOUND_ID);
    emit videoRecordingEndChanged();
  }
}

QString Sounds::autoFocusAcquired() const {
  return m_autoFocusAcquired;
}

void Sounds::setAutoFocusAcquired(const QString& path) {
  if (path != m_autoFocusAcquired) {
    m_autoFocusAcquired = path;
    cache(m_autoFocusAcquired, CAMERA_FOCUS_SOUND_ID);
    emit autoFocusAcquiredChanged();
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
