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

#ifndef QT_CAM_VIEWFINDER_RENDERER_H
#define QT_CAM_VIEWFINDER_RENDERER_H

#include <QObject>
#include <gst/gst.h>
#include <QRectF>

class QtCamConfig;
class QMetaObject;
class QMatrix4x4;
class QSizeF;

class QtCamViewfinderRenderer : public QObject {
  Q_OBJECT

public:
  static QtCamViewfinderRenderer *create(QtCamConfig *config, QObject *parent = 0);
  static int registerRenderer(const QString& key, const QMetaObject& meta);

  virtual ~QtCamViewfinderRenderer();

  virtual void paint(const QMatrix4x4& matrix, const QRectF& viewport) = 0;
  virtual void resize(const QSizeF& size) = 0;
  virtual void reset() = 0;
  virtual void start() = 0;
  virtual GstElement *sinkElement() = 0;

  virtual QRectF renderArea() = 0;
  virtual QSizeF videoResolution() = 0;

  virtual bool needsNativePainting() = 0;

protected:
  QtCamViewfinderRenderer(QtCamConfig *config, QObject *parent = 0);

signals:
  void updateRequested();
  void renderAreaChanged();
  void videoResolutionChanged();
};

#define QT_CAM_VIEWFINDER_RENDERER(key, klass) \
  static int klass_seq = QtCamViewfinderRenderer::registerRenderer(key, klass::staticMetaObject);

#endif /* QT_CAM_VIEWFINDER_RENDERER_H */
