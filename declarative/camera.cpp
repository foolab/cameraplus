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

#include "camera.h"
#include "qtcamera.h"
#include "qtcamdevice.h"
#include "qtcammode.h"
#include "qtcamimagemode.h"
#include "qtcamvideomode.h"
#include "qtcamgraphicsviewfinder.h"
#include "qtcamconfig.h"
#include "declarativeqtcameranotifications.h"
#include "notificationscontainer.h"
#include "sounds.h"
#include <QDeclarativeInfo>

#include "zoom.h"
#include "flash.h"
#include "scene.h"
#include "evcomp.h"
#include "whitebalance.h"
#include "colortone.h"
#include "iso.h"
#include "exposure.h"
#include "aperture.h"
#include "noisereduction.h"
#include "flickerreduction.h"
#include "focus.h"
#include "autofocus.h"
#include "roi.h"

#include "videomute.h"
#include "videotorch.h"
#include "cameraconfig.h"

Camera::Camera(QDeclarativeItem *parent) :
  QDeclarativeItem(parent),
  m_cam(new QtCamera(this)),
  m_dev(0),
  m_vf(new QtCamGraphicsViewfinder(m_cam->config(), this)),
  m_mode(Camera::UnknownMode),
  m_notifications(new NotificationsContainer(this)),
  m_zoom(0),
  m_flash(0),
  m_scene(0),
  m_evComp(0),
  m_whiteBalance(0),
  m_colorTone(0),
  m_iso(0),
  m_exposure(0),
  m_aperture(0),
  m_noiseReduction(0),
  m_flickerReduction(0),
  m_focus(0),
  m_autoFocus(0),
  m_roi(0),
  m_videoMute(0),
  m_videoTorch(0),
  m_config(new CameraConfig(this)) {

  QObject::connect(m_vf, SIGNAL(renderAreaChanged()), this, SIGNAL(renderAreaChanged()));
  QObject::connect(m_vf, SIGNAL(videoResolutionChanged()), this, SIGNAL(videoResolutionChanged()));
  QObject::connect(m_vf, SIGNAL(renderingEnabledChanged()), this, SIGNAL(renderingEnabledChanged()));
}

Camera::~Camera() {
  if (m_dev) {
    if (m_dev->activeMode()) {
      m_dev->activeMode()->deactivate();
    }

    m_dev->stop(true);
    m_dev->deleteLater();
    m_dev = 0;
  }

  delete m_zoom;
  delete m_flash;
  delete m_scene;
  delete m_evComp;
  delete m_whiteBalance;
  delete m_colorTone;
  delete m_iso;
  delete m_exposure;
  delete m_aperture;
  delete m_noiseReduction;
  delete m_flickerReduction;
  delete m_focus;
  delete m_autoFocus;
  delete m_roi;
  delete m_videoMute;
  delete m_videoTorch;
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

      resetCapabilities();
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

  // TODO: seems setting geometry breaks rendering somehow
  //  m_vf->setGeometry(newGeometry);
  m_vf->resize(newGeometry.size());
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

DeclarativeQtCameraNotifications *Camera::notifications() const {
  return m_notifications->notifications();
}

void Camera::setNotifications(DeclarativeQtCameraNotifications *notifications) {
  if (m_notifications->setNotifications(notifications)) {

    if (Sounds *s = dynamic_cast<Sounds *>(notifications)) {
      s->setConfig(m_cam->config());
      s->reload();
    }

    emit notificationsChanged();
  }
}

QRectF Camera::renderArea() const {
  return m_vf->renderArea();
}

QSizeF Camera::videoResolution() const {
  return m_vf->videoResolution();
}

void Camera::resetCapabilities() {
  QtCamDevice *dev = device();

  delete m_zoom;
  m_zoom = new Zoom(dev, this);
  emit zoomChanged();

  delete m_flash;
  m_flash = new Flash(dev, this);
  emit flashChanged();

  delete m_scene;
  m_scene = new Scene(dev, this);
  emit sceneChanged();

  delete m_evComp;
  m_evComp = new EvComp(dev, this);
  emit evCompChanged();

  delete m_whiteBalance;
  m_whiteBalance = new WhiteBalance(dev, this);
  emit whiteBalanceChanged();

  delete m_colorTone;
  m_colorTone = new ColorTone(dev, this);
  emit colorToneChanged();

  delete m_iso;
  m_iso = new Iso(dev, this);
  emit isoChanged();

  delete m_exposure;
  m_exposure = new Exposure(dev, this);
  emit exposureChanged();

  delete m_aperture;
  m_aperture = new Aperture(dev, this);
  emit apertureChanged();

  delete m_noiseReduction;
  m_noiseReduction = new NoiseReduction(dev, this);
  emit noiseReductionChanged();

  delete m_flickerReduction;
  m_flickerReduction = new FlickerReduction(dev, this);
  emit flickerReductionChanged();

  delete m_focus;
  m_focus = new Focus(dev, this);
  emit focusChanged();

  delete m_autoFocus;
  m_autoFocus = new AutoFocus(dev, this);
  emit autoFocusChanged();

  delete m_roi;
  m_roi = new Roi(dev, this);
  emit roiChanged();

  delete m_videoMute;
  m_videoMute = new VideoMute(dev, this);
  emit videoMuteChanged();

  delete m_videoTorch;
  m_videoTorch = new VideoTorch(dev, this);
  emit videoTorchChanged();
}

Zoom *Camera::zoom() const {
  return m_zoom;
}

Flash *Camera::flash() const {
  return m_flash;
}

Scene *Camera::scene() const {
  return m_scene;
}

EvComp *Camera::evComp() const {
  return m_evComp;
}

WhiteBalance *Camera::whiteBalance() const {
  return m_whiteBalance;
}

ColorTone *Camera::colorTone() const {
  return m_colorTone;
}

Iso *Camera::iso() const {
  return m_iso;
}

Exposure *Camera::exposure() const {
  return m_exposure;
}

Aperture *Camera::aperture() const {
  return m_aperture;
}

NoiseReduction *Camera::noiseReduction() const {
  return m_noiseReduction;
}

FlickerReduction *Camera::flickerReduction() const {
  return m_flickerReduction;
}

Focus *Camera::focus() const {
  return m_focus;
}

AutoFocus *Camera::autoFocus() const {
  return m_autoFocus;
}

Roi *Camera::roi() const {
  return m_roi;
}

VideoMute *Camera::videoMute() const {
  return m_videoMute;
}

VideoTorch *Camera::videoTorch() const {
  return m_videoTorch;
}

bool Camera::isRenderingEnabled() const {
  return m_vf->isRenderingEnabled();
}

void Camera::setRenderingEnabled(bool enabled) {
  m_vf->setRenderingEnabled(enabled);
}

CameraConfig *Camera::cameraConfig() const {
  return m_config;
}
