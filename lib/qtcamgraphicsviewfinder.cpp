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

#include "qtcamgraphicsviewfinder.h"
#include "qtcamconfig.h"
#include "qtcamdevice.h"
#include <QDebug>
#include "qtcamviewfinderrenderer.h"
#include <QPainter>
#include <QGraphicsSceneResizeEvent>
#include <QMatrix4x4>

class QtCamGraphicsViewfinderPrivate {
public:
  void ensureBackend() {
    if (!renderer) {
      renderer = QtCamViewfinderRenderer::create(conf, q_ptr);
      if (!renderer) {
	qCritical() << "Failed to create a viewfinder renderer";
	return;
      }

      renderer->resize(q_ptr->size());
      QObject::connect(renderer, SIGNAL(updateRequested()), q_ptr, SLOT(updateRequested()));
      QObject::connect(renderer, SIGNAL(renderAreaChanged()), q_ptr, SIGNAL(renderAreaChanged()));
      QObject::connect(renderer, SIGNAL(videoResolutionChanged()), q_ptr, SIGNAL(videoResolutionChanged()));
    }
  }

  void resetBackend() {
    if (renderer) {
      renderer->reset();
    }
  }

  QtCamViewfinderRenderer *renderer;
  QtCamConfig *conf;
  QtCamDevice *dev;
  QtCamGraphicsViewfinder *q_ptr;
  bool enabled;
};

QtCamGraphicsViewfinder::QtCamGraphicsViewfinder(QtCamConfig *config, QGraphicsItem *parent) :
  QGraphicsWidget(parent), d_ptr(new QtCamGraphicsViewfinderPrivate) {
  d_ptr->conf = config;
  d_ptr->dev = 0;
  d_ptr->renderer = 0;
  d_ptr->q_ptr = this;
  d_ptr->enabled = true;
}

QtCamGraphicsViewfinder::~QtCamGraphicsViewfinder() {
  delete d_ptr->renderer;
  delete d_ptr; d_ptr = 0;
}

GstElement *QtCamGraphicsViewfinder::sinkElement() {
  d_ptr->ensureBackend();

  if (!d_ptr->renderer) {
    return 0;
  }

  return d_ptr->renderer->sinkElement();
}

bool QtCamGraphicsViewfinder::setDevice(QtCamDevice *device) {
  if (device && d_ptr->dev == device) {
    return true;
  }

  if (d_ptr->dev) {
    qWarning() << "QtCamGraphicsViewfinder: viewfinder cannot be replaced.";
    return false;
  }

  if (!device) {
    qWarning() << "QtCamGraphicsViewfinder: viewfinder cannot be unset.";
    return false;
  }

  // This is to break the loop.
  d_ptr->dev = device;
  if (!device->setViewfinder(this)) {
    d_ptr->dev = 0;
    return false;
  }

  d_ptr->resetBackend();

  return true;
}

void QtCamGraphicsViewfinder::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
				    QWidget *widget) {
  Q_UNUSED(widget);
  Q_UNUSED(option);

  painter->fillRect(boundingRect(), Qt::black);

  if (!d_ptr->renderer || !d_ptr->enabled) {
    return;
  }

  bool needsNativePainting = d_ptr->renderer->needsNativePainting();

  if (needsNativePainting) {
    painter->beginNativePainting();
  }

  d_ptr->renderer->paint(QMatrix4x4(painter->combinedTransform()), painter->viewport());

  if (needsNativePainting) {
    painter->endNativePainting();
  }
}

void QtCamGraphicsViewfinder::resizeEvent(QGraphicsSceneResizeEvent *event) {
  QGraphicsWidget::resizeEvent(event);

  if (!d_ptr->renderer) {
    return;
  }

  d_ptr->renderer->resize(event->newSize());
}

void QtCamGraphicsViewfinder::updateRequested() {
  if (d_ptr->enabled) {
    update();
  }
}

void QtCamGraphicsViewfinder::stop() {
  d_ptr->resetBackend();

  setRenderingEnabled(true);
}

QRectF QtCamGraphicsViewfinder::renderArea() const {
  if (!d_ptr->renderer) {
    return QRectF();
  }

  return d_ptr->renderer->renderArea();
}

QSizeF QtCamGraphicsViewfinder::videoResolution() const {
  if (!d_ptr->renderer) {
    return QSizeF();
  }

  return d_ptr->renderer->videoResolution();
}

bool QtCamGraphicsViewfinder::isRenderingEnabled() const {
  return d_ptr->enabled;
}

void QtCamGraphicsViewfinder::setRenderingEnabled(bool enabled) {
  if (d_ptr->enabled != enabled) {
    d_ptr->enabled = enabled;

    emit renderingEnabledChanged();
  }
}
