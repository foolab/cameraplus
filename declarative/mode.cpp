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

#include "mode.h"
#include "qtcammode.h"
#include "camera.h"
#include "qtcamdevice.h"
#include "previewprovider.h"

Mode::Mode(QObject *parent) :
  QObject(parent),
  m_cam(0),
  m_mode(0),
  m_seq(0),
  m_previewEnabled(true) {

}

Mode::~Mode() {
  m_cam = 0;
  m_mode = 0;
}

Camera *Mode::camera() {
  return m_cam;
}

void Mode::setCamera(Camera *camera) {
  if (camera == m_cam) {
    return;
  }

  if (m_cam) {
    QObject::disconnect(m_cam, SIGNAL(prepareForDeviceChange()), this, SLOT(prepareForDeviceChange()));
    QObject::disconnect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
    QObject::disconnect(m_cam, SIGNAL(deviceChanged()), this, SIGNAL(isReadyChanged()));
  }

  m_cam = camera;

  if (m_cam) {
    QObject::connect(m_cam, SIGNAL(prepareForDeviceChange()), this, SLOT(prepareForDeviceChange()));
    QObject::connect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
    QObject::connect(m_cam, SIGNAL(deviceChanged()), this, SIGNAL(isReadyChanged()));
  }

  emit cameraChanged();

  deviceChanged();

  emit isReadyChanged();
}

bool Mode::isActive() {
  return m_mode ? m_mode->isActive() : false;
}

bool Mode::canCapture() {
  return m_mode ? m_mode->canCapture() : false;
}

void Mode::deviceChanged() {
  if (!m_cam || !m_cam->device()) {
    return;
  }

  changeMode();

  if (m_mode) {
    QObject::connect(m_mode, SIGNAL(canCaptureChanged()), this, SIGNAL(canCaptureChanged()));
    QObject::connect(m_mode, SIGNAL(saved(const QString&)), this, SIGNAL(saved(const QString&)));
    QObject::connect(m_mode, SIGNAL(previewAvailable(const QImage&, const QString&)),
		     this, SLOT(gotPreview(const QImage&, const QString&)));
    QObject::connect(m_mode, SIGNAL(activeChanged()), this, SIGNAL(activeChanged()));
    QObject::connect(m_mode, SIGNAL(activeChanged()), this, SIGNAL(canCaptureChanged()));
    QObject::connect(m_cam->device(), SIGNAL(idleStateChanged(bool)),
    		     this, SIGNAL(canCaptureChanged()));
    QObject::connect(m_cam->device(), SIGNAL(runningStateChanged(bool)),
		     this, SIGNAL(canCaptureChanged()));

    setPreviewState();

    postChangeMode();
  }

  emit canCaptureChanged();
  emit activeChanged();
}

void Mode::gotPreview(const QImage& image, const QString& fileName) {
  PreviewProvider::instance()->setPreview(image);

  // HACK: QML insists on caching the images.
  QString url = QString("image://preview/%1").arg(m_seq);
  ++m_seq;

  emit previewAvailable(url, fileName);
}

bool Mode::isReady() const {
  return m_mode;
}

void Mode::prepareForDeviceChange() {
  if (m_mode) {
    QObject::disconnect(m_mode, SIGNAL(canCaptureChanged()), this, SIGNAL(canCaptureChanged()));
    QObject::disconnect(m_mode, SIGNAL(saved(const QString&)),
			this, SIGNAL(saved(const QString&)));
    QObject::disconnect(m_mode, SIGNAL(previewAvailable(const QImage&, const QString&)),
			this, SLOT(gotPreview(const QImage&, const QString&)));
    QObject::disconnect(m_mode, SIGNAL(activeChanged()), this, SIGNAL(activeChanged()));
    QObject::disconnect(m_mode, SIGNAL(activeChanged()), this, SIGNAL(canCaptureChanged()));
    QObject::disconnect(m_cam->device(), SIGNAL(idleStateChanged(bool)),
			this, SIGNAL(canCaptureChanged()));
    QObject::disconnect(m_cam->device(), SIGNAL(runningStateChanged(bool)),
			this, SIGNAL(canCaptureChanged()));

    preChangeMode();
  }
}

bool Mode::isPreviewEnabled() {
  return m_previewEnabled;
}

void Mode::setPreviewEnabled(bool enabled) {
  if (enabled != m_previewEnabled) {
    m_previewEnabled = enabled;

    if (m_mode) {
      setPreviewState();
    }

    emit enablePreviewChanged();
  }
}

void Mode::setPreviewState() {
  if (m_previewEnabled) {
    m_mode->enablePreview();
  }
  else {
    m_mode->disablePreview();
  }
}
