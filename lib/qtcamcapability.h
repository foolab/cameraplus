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

#ifndef QT_CAM_CAPABILITY_H
#define QT_CAM_CAPABILITY_H

#include <QObject>

class QtCamDevice;
class QtCamCapabilityPrivate;

class QtCamCapability : public QObject {
  Q_OBJECT
  Q_FLAGS(Capability Capabilities)

public:
  typedef enum {
    EvComp = (1 << 0),
    IsoSpeed = (1 << 1),
    WhiteBalance = (1 << 2),
    ColourTone = (1 << 3),
    Scene = (1 << 4),
    Flash = (1 << 5),
    Zoom = (1 << 6),
    Focus = (1 << 7),
    Aperture = (1 << 8),
    Exposure = (1 << 9),
    Shake = (1 << 10),
    NoiseReduction = (1 << 11),
    FlickerReduction = (1 << 12)
  } Capability;

  Q_DECLARE_FLAGS(Capabilities, Capability)

  static Capabilities capabilities(QtCamDevice *dev);

  QtCamCapability(QtCamCapabilityPrivate *d, QObject *parent = 0);
  virtual ~QtCamCapability();

  bool isSupported();

signals:
  void valueChanged();
  void minimumValueChanged();
  void maximumValueChanged();

protected:
  QtCamCapabilityPrivate *d_ptr;
};

#endif /* QT_CAM_CAPABILITY_H */
