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

#ifndef POSITION_H
#define POSITION_H

#include <QObject>
#include <QRectF>

class Position : public QObject {
  Q_OBJECT

  Q_PROPERTY(int deviceOrientationAngle READ deviceOrientationAngle WRITE setDeviceOrientationAngle NOTIFY deviceOrientationAngleChanged);
  Q_PROPERTY(int sensorOrientationAngle READ sensorOrientationAngle WRITE setSensorOrientationAngle NOTIFY sensorOrientationAngleChanged);
  Q_PROPERTY(bool frontCamera READ isFrontCamera WRITE setFrontCamera NOTIFY isFrontCameraChanged);
  Q_PROPERTY(int applicationOrientationAngle READ applicationOrientationAngle WRITE setApplicationOrientationAngle NOTIFY applicationOrientationAngleChanged);
  Q_PROPERTY(NaturalOrientation naturalOrientation READ naturalOrientation WRITE setNaturalOrientation NOTIFY naturalOrientationChanged);

  Q_PROPERTY(int viewfinderRotationAngle READ viewfinderRotationAngle NOTIFY viewfinderRotationAngleChanged);
  Q_PROPERTY(bool isViewfinderFlipped READ isViewfinderFlipped NOTIFY isViewfinderFlippedChanged);
  Q_PROPERTY(int exifRotationAngle READ exifRotationAngle NOTIFY exifRotationAngleChanged);
  Q_PROPERTY(bool isExifFlipped READ isExifFlipped NOTIFY isExifFlippedChanged);
  Q_PROPERTY(bool isPortrait READ isPortrait NOTIFY isPortraitChanged);
  Q_ENUMS(NaturalOrientation);

public:
  Position(QObject *parent = 0);
  ~Position();

  typedef enum {
    Portrait = 0,
    Landscape = 1,
  } NaturalOrientation;

  int deviceOrientationAngle() const;
  void setDeviceOrientationAngle(int angle);

  int sensorOrientationAngle() const;
  void setSensorOrientationAngle(int angle);

  bool isFrontCamera() const;
  void setFrontCamera(bool isFront);

  int applicationOrientationAngle() const;
  void setApplicationOrientationAngle(int angle);

  NaturalOrientation naturalOrientation() const;
  void setNaturalOrientation(NaturalOrientation orientation);

  int viewfinderRotationAngle() const;
  void setViewfinderRotationAngle(int angle);

  bool isViewfinderFlipped() const;
  void setViewfinderFlipped(bool flipped);

  int exifRotationAngle() const;
  void setExifRotationAngle(int angle);

  bool isExifFlipped() const;
  void setExifFlipped(bool flipped);

  bool isPortrait() const;
  void setPortrait(bool portrait);

  Q_INVOKABLE QRectF fromSensorCoordinates(const QRectF& rect, const QPointF& center) const;
  Q_INVOKABLE QRectF toSensorCoordinates(const QRectF& rect, const QPointF& center) const;

signals:
  void deviceOrientationAngleChanged();
  void sensorOrientationAngleChanged();
  void isFrontCameraChanged();
  void applicationOrientationAngleChanged();
  void naturalOrientationChanged();
  void viewfinderRotationAngleChanged();
  void isViewfinderFlippedChanged();
  void exifRotationAngleChanged();
  void isExifFlippedChanged();
  void isPortraitChanged();

private:
  void update();

  int m_deviceOrientationAngle;
  int m_sensorOrientationAngle;
  bool m_isFront;
  int m_applicationOrientationAngle;
  NaturalOrientation m_naturalOrientation;

  int m_viewfinderRotationAngle;
  int m_exifRotationAngle;
  bool m_exifFlipped;
  bool m_isPortrait;
  bool m_viewfinderFlipped;
};

#endif /* POSITION_H */
