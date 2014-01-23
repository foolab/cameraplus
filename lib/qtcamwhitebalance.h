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

#ifndef QT_CAM_WHITE_BALANCE_H
#define QT_CAM_WHITE_BALANCE_H

#include "qtcamcapability.h"

class QtCamWhiteBalance : public QtCamCapability {
  Q_OBJECT

public:
  typedef enum {
    Auto = 0,
    Daylight = 1,
    Cloudy = 2,
    Sunset = 3,
    Tungsten = 4,
    Flourescent = 5
  } WhiteBalanceMode;

  QtCamWhiteBalance(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamWhiteBalance();

  WhiteBalanceMode value();
  bool setValue(const WhiteBalanceMode& mode);
};

#endif /* QT_CAM_WHITE_BALANCE_H */
