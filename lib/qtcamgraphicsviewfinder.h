// -*- c++ -*-

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

#ifndef QT_CAM_GRAPHICS_VIEWFINDER_H
#define QT_CAM_GRAPHICS_VIEWFINDER_H

#include "qtcamviewfinder.h"
#include <QGraphicsWidget>

class QtCamGraphicsViewfinderPrivate;
class QtCamConfig;

class QtCamGraphicsViewfinder : public QGraphicsWidget, public QtCamViewfinder {
  Q_OBJECT

public:
  QtCamGraphicsViewfinder(QtCamConfig *config, QGraphicsItem *parent = 0);
  virtual ~QtCamGraphicsViewfinder();

  virtual GstElement *sinkElement();
  virtual void stop();

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		     QWidget *widget = 0);

  virtual QtCamViewfinderRenderer *renderer() const;

  QRectF renderArea() const;
  QSizeF videoResolution() const;

  bool isRenderingEnabled() const;
  void setRenderingEnabled(bool enabled);

signals:
  void renderAreaChanged();
  void videoResolutionChanged();
  void renderingEnabledChanged();

protected:
  void resizeEvent(QGraphicsSceneResizeEvent *event);

private slots:
  void updateRequested();

private:
  QtCamGraphicsViewfinderPrivate *d_ptr;
};

#endif /* QT_CAM_GRAPHICS_VIEWFINDER_H */
