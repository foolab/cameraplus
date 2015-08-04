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

#include "viewfinderhandler.h"
#include "camera.h"
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

ViewfinderHandler::ViewfinderHandler(const char *slot, QObject *parent) :
  QObject(parent),
  m_slot(QMetaObject::normalizedSignature(slot)),
  m_cam(0),
  m_handler(0),
  m_dev(0),
  m_enabled(false) {

}

ViewfinderHandler::~ViewfinderHandler() {
  // subclass should call
  // deviceAboutToChange();
  // because it's an abstract virtual method.
}

Camera *ViewfinderHandler::camera() const {
  return m_cam;
}

void ViewfinderHandler::setCamera(Camera *camera) {
  if (m_cam == camera) {
    return;
  }

  bool enabled = m_enabled;
  m_enabled = false;

  QMutexLocker l(&m_mutex);

  if (m_cam) {
    QObject::disconnect(m_cam, SIGNAL(prepareForDeviceChange()),
			this, SLOT(deviceAboutToChange()));
    QObject::disconnect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));

    update();
  }

  m_cam = camera;

  m_enabled = enabled;

  if (m_cam) {
    QObject::connect(m_cam, SIGNAL(prepareForDeviceChange()), this, SLOT(deviceAboutToChange()));
    QObject::connect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
  }

  emit cameraChanged();

  update();
}

QObject *ViewfinderHandler::handler() const {
  return m_handler;
}

void ViewfinderHandler::setHandler(QObject *handler) {
  if (m_handler != handler) {

    QMutexLocker l(&m_mutex);

    bool enabled = m_enabled;
    m_enabled = false;

    update();

    m_handler = handler;
    emit handlerChanged();

    // now resolve our meta method:
    m_method = QMetaMethod();
    const QMetaObject *obj = m_handler->metaObject();
    int index = obj->indexOfSlot(m_slot.toUtf8().constData());
    if (index == -1) {
      qmlInfo(this) << "Cannot find slot " << m_slot;
    } else {
      m_method = obj->method(index);
    }

    m_enabled = enabled;

    update();
  }
}

void ViewfinderHandler::deviceChanged() {
  QMutexLocker l(&m_mutex);
  update();
}

void ViewfinderHandler::deviceAboutToChange() {
  QMutexLocker l(&m_mutex);

  bool enabled = m_enabled;
  m_enabled = false;

  update();

  m_enabled = enabled;
}

bool ViewfinderHandler::isEnabled() const {
  return m_enabled;
}

void ViewfinderHandler::setEnabled(bool enabled) {
  if (enabled != isEnabled()) {
    QMutexLocker l(&m_mutex);

    m_enabled = enabled;

    emit enabledChanged();

    update();
  }
}

void ViewfinderHandler::update() {
  bool valid =
#if defined(QT4)
    m_method.signature();
#else
  m_method.isValid();
#endif

  if (m_enabled && m_cam && m_handler && valid) {
    QtCamDevice *dev = m_cam->device();
    if (dev) {
      registerHandler(dev);
    }
  } else {
    if (m_cam) {
      QtCamDevice *dev = m_cam->device();
      if (dev) {
	unregisterHandler(dev);
      }
    }
  }
}
