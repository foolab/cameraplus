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

#include "camera.h"
#include "qtcamera.h"
#include "qtcamdevice.h"
#include "qtcammode.h"
#include "qtcamimagemode.h"
#include "qtcamvideomode.h"
#include "qtcamgraphicsviewfinder.h"
#include "qtcamconfig.h"
#include "notifications.h"
#include "notificationscontainer.h"
#include "sounds.h"
#include <QDeclarativeInfo>

// TODO: a viewfinder class that inherits QDeclarativeItem

Camera::Camera(QDeclarativeItem *parent) :
  QDeclarativeItem(parent),
  m_cam(new QtCamera(this)),
  m_dev(0),
  m_vf(new QtCamGraphicsViewfinder(m_cam->config(), this)),
  m_mode(Camera::UnknownMode),
  m_notifications(new NotificationsContainer(this)) {

  // TODO:
}

Camera::~Camera() {
  if (m_dev) {
    m_dev->stop(true);
    m_dev->deleteLater();
    m_dev = 0;
  }

  // TODO: cleanup
}

void Camera::componentComplete() {
  QDeclarativeItem::componentComplete();

  emit deviceCountChanged();
}

int Camera::deviceCount() const {
  return m_cam ? m_cam->devices().size() : 0;
}

QString Camera::deviceName(int index) const {
  return m_cam->devices().at(index).first;
}

QVariant Camera::deviceId(int index) const {
  return m_cam->devices().at(index).second;
}

bool Camera::reset(const QVariant& deviceId, const CameraMode& mode) {
  if (mode == Camera::UnknownMode) {
    qmlInfo(this) << "Cannot set mode to unknown";
    return false;
  }

  if (!isComponentComplete()) {
    qmlInfo(this) << "Component is still not ready";
    return false;
  }

  QVariant oldId = m_id;
  Camera::CameraMode oldMode = m_mode;

  if (setDeviceId(deviceId) && setMode(mode)) {
    if (oldId != m_id) {
      emit deviceIdChanged();
      emit deviceChanged();
    }

    if (oldMode != m_mode) {
      emit modeChanged();
    }

    return true;
  }

  return false;
}

bool Camera::setDeviceId(const QVariant& deviceId) {
  if (deviceId == m_id) {
    return true;
  }

  if (m_dev && m_dev->stop(false)) {
    delete m_dev;
  }
  else if (m_dev) {
    qmlInfo(this) << "Failed to stop device";
    return false;
  }

  m_dev = m_cam->device(deviceId, this);

  m_id = deviceId;

  m_vf->setDevice(m_dev);

  QObject::connect(m_dev, SIGNAL(runningStateChanged(bool)),
		      this, SIGNAL(runningStateChanged()));
  QObject::connect(m_dev, SIGNAL(idleStateChanged(bool)), this, SIGNAL(idleStateChanged()));
  QObject::connect(m_dev, SIGNAL(error(const QString&, int, const QString&)),
		   this, SIGNAL(error(const QString&, int, const QString&)));

  m_notifications->setDevice(m_dev);

  return true;
}

QVariant Camera::deviceId() const {
  return m_id;
}

void Camera::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);

  m_vf->setGeometry(newGeometry);
}

QtCamDevice *Camera::device() const {
  return m_dev;
}

bool Camera::setMode(const Camera::CameraMode& mode) {
  if (m_mode == mode) {
    return true;
  }

  if (!m_dev) {
    return false;
  }

  m_mode = mode;

  if (m_dev->isRunning()) {
    applyMode();
  }

  return true;
}

Camera::CameraMode Camera::mode() {
  return m_mode;
}

bool Camera::start() {
  if (!m_dev) {
    return false;
  }

  if (!applyMode()) {
    return false;
  }

  return m_dev->start();
}

bool Camera::stop(bool force) {
  if (m_dev) {
    return m_dev->stop(force);
  }

  return true;
}

bool Camera::isIdle() {
  return m_dev ? m_dev->isIdle() : true;
}

bool Camera::isRunning() {
  return m_dev ? m_dev->isRunning() : false;
}

bool Camera::applyMode() {
  if (m_mode == Camera::UnknownMode) {
    return false;
  }

  if (m_mode == Camera::VideoMode && m_dev->activeMode() != m_dev->videoMode()) {
    m_dev->videoMode()->activate();
  }
  else if (m_mode == Camera::ImageMode && m_dev->activeMode() != m_dev->imageMode()) {
    m_dev->imageMode()->activate();
  }

  return true;
}

QString Camera::imageSuffix() const {
  return m_cam->config()->imageSuffix();
}

QString Camera::videoSuffix() const {
  return m_cam->config()->videoSuffix();
}

Notifications *Camera::notifications() const {
  return m_notifications->notifications();
}

void Camera::setNotifications(Notifications *notifications) {
  if (m_notifications->setNotifications(notifications)) {

    if (Sounds *s = dynamic_cast<Sounds *>(notifications)) {
      s->setConfig(m_cam->config());
      s->reload();
    }

    emit notificationsChanged();
  }
}
