// -*- c++ -*-

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

#include "viewfinder.h"
#include "camera.h"
#include "cameraconfig.h"
#include "qtcamviewfinderrenderer.h"
#include <QDeclarativeInfo>
#include <QPainter>
#include "qtcamdevice.h"

Viewfinder::Viewfinder(QDeclarativeItem *parent) :
  QDeclarativeItem(parent),
  m_renderer(0),
  m_cam(0),
  m_conf(0),
  m_enabled(true) {

  setFlag(QGraphicsItem::ItemHasNoContents, false);
}

Viewfinder::~Viewfinder() {

}

bool Viewfinder::isRenderingEnabled() const {
  return m_enabled;
}

void Viewfinder::setRenderingEnabled(bool enabled) {
  if (m_enabled != enabled) {
    m_enabled = enabled;
    emit renderingEnabledChanged();
  }
}

Camera *Viewfinder::camera() const {
  return m_cam;
}

void Viewfinder::setCamera(Camera *camera) {
  if (m_cam == camera) {
    return;
  }

  if (m_cam && m_cam != camera) {
    qmlInfo(this) << "Cannot reset Camera";
    return;
  }

  if (!camera) {
    qmlInfo(this) << "Camera cannot be empty";
    return;
  }

  if (m_cam) {
    QObject::disconnect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
  }

  m_cam = camera;

  if (m_cam) {
    QObject::connect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
  }

  if (isComponentComplete()) {
    deviceChanged();
  }

  emit cameraChanged();
}

CameraConfig *Viewfinder::cameraConfig() const {
  return m_conf;
}

void Viewfinder::setCameraConfig(CameraConfig *config) {
  if (m_conf && m_conf != config) {
    qmlInfo(this) << "Cannot reset CameraConfig";
    return;
  }

  if (!config) {
    qmlInfo(this) << "CameraConfig cannot be empty";
    return;
  }

  if (m_conf == config) {
    return;
  }

  m_conf = config;

  if (isComponentComplete()) {
    deviceChanged();
  }

  emit cameraConfigChanged();
}

void Viewfinder::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		       QWidget *widget) {

  Q_UNUSED(widget);
  Q_UNUSED(option);

  painter->fillRect(boundingRect(), Qt::black);

  if (!m_renderer || !m_enabled) {
    return;
  }

  m_renderer->paint(painter);
}

QRectF Viewfinder::renderArea() const {
  return m_renderer ? m_renderer->renderArea() : QRectF();
}

QSizeF Viewfinder::videoResolution() const {
  return m_renderer ? m_renderer->videoResolution() : QSizeF();
}

void Viewfinder::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);

  if (m_renderer) {
    m_renderer->resize(newGeometry.size());
  }
}

void Viewfinder::componentComplete() {
  if (!m_cam) {
    qmlInfo(this) << "Camera not set";
    return;
  }

  if (!m_conf) {
    qmlInfo(this) << "CameraConfig not set";
    return;
  }

  m_renderer = QtCamViewfinderRenderer::create(m_conf->config(), this);
  if (!m_renderer) {
    qmlInfo(this) << "Failed to create viewfinder renderer";
    return;
  }

  m_renderer->resize(QSizeF(width(), height()));
  QObject::connect(m_renderer, SIGNAL(updateRequested()), this, SLOT(updateRequested()));
  QObject::connect(m_renderer, SIGNAL(renderAreaChanged()), this, SIGNAL(renderAreaChanged()));
  QObject::connect(m_renderer, SIGNAL(videoResolutionChanged()), this, SIGNAL(videoResolutionChanged()));
}

void Viewfinder::deviceChanged() {
  if (!m_renderer) {
    qmlInfo(this) << "No renderer";
    return;
  }

  QtCamDevice *dev = m_cam->device();

  if (dev) {
    dev->setViewfinder(this);
  }

  emit renderAreaChanged();
  emit videoResolutionChanged();
}

GstElement *Viewfinder::sinkElement() {
  return m_renderer ? m_renderer->sinkElement() : 0;
}

bool Viewfinder::setDevice(QtCamDevice *device) {
  Q_UNUSED(device);

  return true;
}

void Viewfinder::stop() {
  if (m_renderer) {
    m_renderer->reset();
  }
}

void Viewfinder::updateRequested() {
  if (m_enabled) {
    update();
  }
}
