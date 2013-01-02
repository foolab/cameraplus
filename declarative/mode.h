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

#ifndef MODE_H
#define MODE_H

#include <QObject>

class Camera;
class QImage;
class QtCamMode;

class Mode : public QObject {
  Q_OBJECT

  Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged);
  Q_PROPERTY(bool canCapture READ canCapture NOTIFY canCaptureChanged);
  Q_PROPERTY(bool active READ isActive NOTIFY activeChanged);
  Q_PROPERTY(bool ready READ isReady NOTIFY isReadyChanged);

public:
  Mode(QObject *parent = 0);
  virtual ~Mode();

  Camera *camera();
  virtual void setCamera(Camera *camera);

  bool isActive();

  bool canCapture();

  bool isReady() const;

signals:
  void cameraChanged();
  void canCaptureChanged();
  void activeChanged();
  void previewAvailable(const QString& preview, const QString& fileName);
  void saved(const QString& fileName);
  void isReadyChanged();

private slots:
  void gotPreview(const QImage& image, const QString& fileName);
  void deviceChanged();

protected:
  virtual void preChangeMode() = 0;
  virtual void postChangeMode() = 0;
  virtual void changeMode() = 0;

  Camera *m_cam;
  QtCamMode *m_mode;

private:
  unsigned long long m_seq;
};

#endif /* MODE_H */
