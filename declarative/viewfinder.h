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

#ifndef VIEWFINDER_H
#define VIEWFINDER_H

#if defined(QT4)
#include <QDeclarativeItem>
#elif defined(QT5)
#include <QQuickPaintedItem>
#endif
#include "qtcamviewfinder.h"

class QtCamViewfinderRenderer;
class Camera;
class CameraConfig;
class QtCamDevice;

#if defined(QT4)
class Viewfinder : public QDeclarativeItem, public QtCamViewfinder {
#elif defined(QT5)
class Viewfinder : public QQuickPaintedItem, public QtCamViewfinder {
#endif

  Q_OBJECT

  Q_PROPERTY(QRectF renderArea READ renderArea NOTIFY renderAreaChanged);
  Q_PROPERTY(QSizeF videoResolution READ videoResolution NOTIFY videoResolutionChanged);
  Q_PROPERTY(bool renderingEnabled READ isRenderingEnabled WRITE setRenderingEnabled NOTIFY renderingEnabledChanged);
  Q_PROPERTY(Camera *camera READ camera WRITE setCamera NOTIFY cameraChanged);
  Q_PROPERTY(CameraConfig *cameraConfig READ cameraConfig WRITE setCameraConfig NOTIFY cameraConfigChanged);
  Q_PROPERTY(int viewfinderRotationAngle READ viewfinderRotationAngle WRITE setViewfinderRotationAngle NOTIFY viewfinderRotationAngleChanged);
  Q_PROPERTY(bool viewfinderFlipped READ viewfinderFlipped WRITE setViewfinderFlipped NOTIFY viewfinderFlippedChanged);

public:
#if defined(QT4)
  Viewfinder(QDeclarativeItem *parent = 0);
#elif defined(QT5)
  Viewfinder(QQuickItem *parent = 0);
#endif

  ~Viewfinder();

  QRectF renderArea() const;
  QSizeF videoResolution() const;

  bool isRenderingEnabled() const;
  void setRenderingEnabled(bool enabled);

  Camera *camera() const;
  void setCamera(Camera *camera);

  CameraConfig *cameraConfig() const;
  void setCameraConfig(CameraConfig *config);

  int viewfinderRotationAngle() const;
  void setViewfinderRotationAngle(int angle);

  bool viewfinderFlipped() const;
  void setViewfinderFlipped(bool flipped);

#if defined(QT4)
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
#elif defined(QT5)
  void paint(QPainter *painter);
#endif

  GstElement *sinkElement();
  void start();
  void stop();

signals:
  void renderAreaChanged();
  void videoResolutionChanged();
  void renderingEnabledChanged();
  void cameraChanged();
  void cameraConfigChanged();
  void viewfinderRotationAngleChanged();
  void viewfinderFlippedChanged();

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);
  void componentComplete();

private slots:
  void deviceChanged();
  void updateRequested();
  void prepareForDeviceChange();

private:
  void createRenderer();

  QtCamViewfinderRenderer *m_renderer;
  Camera *m_cam;
  CameraConfig *m_conf;
  QtCamDevice *m_dev;
  bool m_enabled;
  int m_angle;
  bool m_flipped;
};

#endif /* VIEWFINDER_H */
