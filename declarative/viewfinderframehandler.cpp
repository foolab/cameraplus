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

#include "viewfinderframehandler.h"
#include "qtcamdevice.h"
#include "qtcamviewfinderframelistener.h"
#include "camera.h"
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

ViewfinderFrameHandler::ViewfinderFrameHandler(QObject *parent) :
  QObject(parent),
  m_cam(0),
  m_handler(0),
  m_dev(0) {

}

ViewfinderFrameHandler::~ViewfinderFrameHandler() {
  deviceAboutToChange();
}

Camera *ViewfinderFrameHandler::camera() const {
  return m_cam;
}

void ViewfinderFrameHandler::setCamera(Camera *camera) {
  if (m_cam == camera) {
    return;
  }

  if (m_cam) {
    QObject::disconnect(m_cam, SIGNAL(prepareForDeviceChange()),
			this, SLOT(deviceAboutToChange()));
    QObject::disconnect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));

    deviceAboutToChange();
  }

  m_cam = camera;

  if (m_cam) {
    QObject::connect(m_cam, SIGNAL(prepareForDeviceChange()), this, SLOT(deviceAboutToChange()));
    QObject::connect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));

    deviceChanged();
  }

  emit cameraChanged();
}

QObject *ViewfinderFrameHandler::handler() const {
  return m_handler;
}

void ViewfinderFrameHandler::setHandler(QObject *handler) {
  if (m_handler != handler) {
    QMutexLocker l(&m_mutex);
    m_handler = handler;
    emit handlerChanged();

    // now resolve our meta method:
    m_method = QMetaMethod();
    const QMetaObject *obj = m_handler->metaObject();
    int index =
      obj->indexOfSlot(QMetaObject::normalizedSignature("handleFrame(const QtCamViewfinderFrame *)"));
    if (index == -1) {
      qmlInfo(this) << "invalid handler";
    } else {
      m_method = obj->method(index);
    }
  }
}

void ViewfinderFrameHandler::deviceChanged() {
  if (m_cam && m_cam->device()) {
    m_cam->device()->frameListener()->addHandler(this);
  }
}

void ViewfinderFrameHandler::deviceAboutToChange() {
  if (m_cam && m_cam->device()) {
    m_cam->device()->frameListener()->removeHandler(this);
  }
}

void ViewfinderFrameHandler::handleFrame(const QtCamViewfinderFrame *frame) {
  QMutexLocker l(&m_mutex);

  if (!m_method.enclosingMetaObject()) {
    return;
  }

  if (!m_method.invoke(m_handler, Qt::DirectConnection, Q_ARG(const QtCamViewfinderFrame *, frame))) {
    qmlInfo(this) << "Failed to invoke handler";
  }
}
