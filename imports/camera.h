// -*- c++ -*-

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

#ifndef CAMERA_H
#define CAMERA_H

#include <QDeclarativeItem>
#include <QVariant>

class QtCamera;
class QtCamDevice;
class QtCamGraphicsViewfinder;

class Camera : public QDeclarativeItem {
  Q_OBJECT

  Q_PROPERTY(int deviceCount READ deviceCount NOTIFY deviceCountChanged)
  Q_PROPERTY(QVariant deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged);
  Q_PROPERTY(CameraMode mode READ mode WRITE setMode NOTIFY modeChanged);
  Q_PROPERTY(bool idle READ isIdle NOTIFY idleStateChanged);
  Q_PROPERTY(bool running READ isRunning NOTIFY runningStateChanged);
  Q_PROPERTY(QString imageSuffix READ imageSuffix CONSTANT);
  Q_PROPERTY(QString videoSuffix READ videoSuffix CONSTANT);
  Q_ENUMS(CameraMode);

public:
  typedef enum {
    ImageMode,
    VideoMode
  } CameraMode;

  Camera(QDeclarativeItem *parent = 0);
  ~Camera();

  virtual void componentComplete();

  int deviceCount() const;
  Q_INVOKABLE QString deviceName(int index) const;
  Q_INVOKABLE QVariant deviceId(int index) const;

  void setDeviceId(const QVariant& id);
  QVariant deviceId() const;

  void setMode(const CameraMode& mode);
  CameraMode mode();

  QtCamDevice *device() const;

  Q_INVOKABLE bool start();
  Q_INVOKABLE bool stop();

  bool isIdle();
  bool isRunning();

  QString imageSuffix() const;
  QString videoSuffix() const;

signals:
  void deviceCountChanged();
  void deviceIdChanged();
  void deviceChanged();
  void modeChanged();
  void idleStateChanged();
  void runningStateChanged();
  void error(const QString& message, int code, const QString& debug);

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

private:
  void applyMode();

  QtCamera *m_cam;
  QtCamDevice *m_dev;
  QVariant m_id;
  QtCamGraphicsViewfinder *m_vf;
  CameraMode m_mode;
};

#endif /* CAMERA_H */
