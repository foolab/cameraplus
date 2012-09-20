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

#ifndef QT_CAM_MODE_H
#define QT_CAM_MODE_H

#include <QObject>
#include <QPair>

class QtCamModePrivate;
class QtCamDevicePrivate;
class QSize;
class QImage;
class QtCamDevice;

class QtCamMode : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool canCapture READ canCapture NOTIFY canCaptureChanged);
  Q_PROPERTY(bool active READ isActive NOTIFY activeChanged);

public:
  QtCamMode(QtCamModePrivate *d, const char *mode, const char *done, QObject *parent = 0);
  virtual ~QtCamMode();

  void deactivate();

  virtual bool canCapture();

  bool isActive();

  virtual void applySettings() = 0;

  QtCamDevice *device() const;

public slots:
  void activate();

signals:
  void previewAvailable(const QImage& image, const QString& fileName);
  void saved(const QString& fileName);
  void canCaptureChanged();
  void activeChanged();

protected:
  virtual void start() = 0;
  virtual void stop() = 0;

  // TODO: move this from here
  void setPreviewSize(const QSize& size);

  // TODO: move this from here
  void setFileName(const QString& fileName);

  QtCamModePrivate *d_ptr;
};

#endif /* QT_CAM_MODE_H */
