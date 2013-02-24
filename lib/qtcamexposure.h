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

#ifndef QT_CAM_EXPOSURE_H
#define QT_CAM_EXPOSURE_H

#include "qtcamcapability.h"

class QtCamExposure : public QtCamCapability {
  Q_OBJECT

public:
  QtCamExposure(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamExposure();

  unsigned int defaultValue();

  unsigned int value();
  bool setValue(unsigned int val);

  unsigned int minimumValue();
  unsigned int maximumValue();
};

#endif /* QT_CAM_EXPOSURE_H */
