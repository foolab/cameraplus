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

#ifndef FLASH_H
#define FLASH_H

#include <QObject>
#include "qtcamflash.h"

class QtCamDevice;

class Flash : public QObject {
  Q_OBJECT

  Q_PROPERTY(FlashMode value READ value WRITE setValue NOTIFY valueChanged);
  Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged);
  Q_ENUMS(FlashMode);

public:
  typedef enum {
    Auto = QtCamFlash::Auto,
    On = QtCamFlash::On,
    Off = QtCamFlash::Off,
    FillIn = QtCamFlash::FillIn,
    RedEye = QtCamFlash::RedEye
  } FlashMode;

  Flash(QtCamDevice *dev, QObject *parent = 0);
  ~Flash();

  FlashMode value();
  void setValue(const FlashMode& mode);

  bool isReady() const;

signals:
  void valueChanged();
  void readyChanged();

private:
  QtCamFlash *m_flash;
};

#endif /* FLASH_H */
