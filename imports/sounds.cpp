/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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

#define CAMERA_IMAGE_START_SOUND_ID  "camera-image-start"
#define CAMERA_IMAGE_END_SOUND_ID    "camera-image-end"
#define CAMERA_VIDEO_START_SOUND_ID  "camera-video-start"
#define CAMERA_VIDEO_STOP_SOUND_ID   "camera-video-stop"
#define CAMERA_FOCUS_SOUND_ID        "camera-focus"

// Odd, volume has to be a char *
#define CANBERRA_FULL_VOLUME "0.0"

// TODO: if we are using headphones then sound volume might be loud. Detect and lower it.

Sounds::Sounds(QObject *parent) :
  QObject(parent),
  m_muted(false),
  m_ctx(0),
  m_conf(0),
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

void Sounds::setConfig(QtCamConfig *conf) {
  m_conf = conf;
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

void Sounds::imageCaptureStarted() {
  if (isMuted() || !m_ctx) {
    return;
  }

  play(CAMERA_IMAGE_START_SOUND_ID);
}

void Sounds::imageCaptureEnded() {
  if (isMuted() || !m_ctx) {
    return;
  }

  play(CAMERA_IMAGE_END_SOUND_ID);
}

void Sounds::videoRecordingStarted() {
  if (isMuted() || !m_ctx) {
    return;
  }

  playAndBlock(CAMERA_VIDEO_START_SOUND_ID);
}

void Sounds::videoRecordingEnded() {
  if (isMuted() || !m_ctx) {
    return;
  }

  play(CAMERA_VIDEO_STOP_SOUND_ID);
}

void Sounds::autoFocusAcquired() {
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

  cache(m_conf->imageCaptureStartedSound(), CAMERA_IMAGE_START_SOUND_ID);
  cache(m_conf->imageCaptureEndedSound(), CAMERA_IMAGE_END_SOUND_ID);
  cache(m_conf->videoRecordingStartedSound(), CAMERA_VIDEO_START_SOUND_ID);
  cache(m_conf->videoRecordingEndedSound(), CAMERA_VIDEO_STOP_SOUND_ID);
  cache(m_conf->autoFocusAcquiredSound(), CAMERA_FOCUS_SOUND_ID);
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

  int code = ca_context_play(m_ctx, 0,
			     CA_PROP_CANBERRA_VOLUME, CANBERRA_FULL_VOLUME,
			     CA_PROP_EVENT_ID, id,
			     CA_PROP_MEDIA_ROLE, "camera-sound-effect",
			     NULL);
  if (code != CA_SUCCESS) {
    qDebug() << "Failed to play sound" << ca_strerror(code) << code;
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
