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

#define CAMERA_IMAGE_START_SOUND_ID "camera-image-start"
#define CAMERA_IMAGE_END_SOUND_ID "camera-image-end"
#define CAMERA_VIDEO_START_SOUND_ID "camera-video-start"
#define CAMERA_VIDEO_STOP_SOUND_ID "camera-video-stop"

// TODO: video start sound should block
// TODO: video end sound should not appear in the video
// TODO: monitor pulse audio death and re-upload our samples
// TODO: if we are using headphones then sound volume might be loud. Detect and lower it.
Sounds::Sounds(QObject *parent) :
  QObject(parent),
  m_muted(false),
  m_ctx(0),
  m_conf(0) {

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

  play(CAMERA_VIDEO_START_SOUND_ID);
}

void Sounds::videoRecordingEnded() {
  if (isMuted() || !m_ctx) {
    return;
  }

  play(CAMERA_VIDEO_STOP_SOUND_ID);
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

  if (int code = ca_context_create(&m_ctx) != CA_SUCCESS) {
    qWarning() << "Failed to create canberra context" << ca_strerror(code) << code;
    return;
  }

  if (int code = ca_context_set_driver(m_ctx, "pulse") != CA_SUCCESS) {
    qWarning() << "Failed to set canberra driver to pulse audio" << ca_strerror(code) << code;
  }

  if (int code = ca_context_change_props(m_ctx,
					 CA_PROP_MEDIA_ROLE, "camera-sound-effect",
					 NULL) != CA_SUCCESS) {
    qWarning() << "Failed to set context properties" << ca_strerror(code) << code;
  }

  if (int code = ca_context_open(m_ctx) != CA_SUCCESS) {
    qWarning() << "Failed to open canberra context" << ca_strerror(code) << code;
    ca_context_destroy(m_ctx);
    m_ctx = 0;
    return;
  }

  cache(m_conf->imageCaptureStartedSound(), CAMERA_IMAGE_START_SOUND_ID);
  cache(m_conf->imageCaptureEndedSound(), CAMERA_IMAGE_END_SOUND_ID);
  cache(m_conf->videoRecordingStartedSound(), CAMERA_VIDEO_START_SOUND_ID);
  cache(m_conf->videoRecordingEndedSound(), CAMERA_VIDEO_STOP_SOUND_ID);
}

void Sounds::cache(const QString& path, const char *id) {
  if (path.isEmpty()) {
    return;
  }

  if (int code = ca_context_cache(m_ctx,
				  CA_PROP_EVENT_ID, id,
				  CA_PROP_MEDIA_FILENAME, path.toLocal8Bit().data(),
				  CA_PROP_CANBERRA_CACHE_CONTROL, "permanent",
				  NULL) != CA_SUCCESS) {
    qWarning() << "Failed to cache" << path << ca_strerror(code) << code;
  }
}

void Sounds::play(const char *id) {
  if (int code = ca_context_play(m_ctx, 0,
				 CA_PROP_CANBERRA_VOLUME, "0.0", // Odd, volume has to be a char *
				 CA_PROP_EVENT_ID, id,
				 CA_PROP_MEDIA_ROLE, "camera-sound-effect",
				 NULL) != CA_SUCCESS) {
    qDebug() << "Failed to play sound" << ca_strerror(code) << code;
  }
}
