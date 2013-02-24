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

#include "qtcamwhitebalance.h"
#include "qtcamcapability_p.h"

QtCamWhiteBalance::QtCamWhiteBalance(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::WhiteBalance,
					     "white-balance-mode"),
		  parent) {

}

QtCamWhiteBalance::~QtCamWhiteBalance() {

}

QtCamWhiteBalance::WhiteBalanceMode QtCamWhiteBalance::value() {
  int val = 0;
  if (!d_ptr->intValue(&val)) {
    return QtCamWhiteBalance::Auto;
  }

  switch (val) {
  case QtCamWhiteBalance::Daylight:
  case QtCamWhiteBalance::Cloudy:
  case QtCamWhiteBalance::Sunset:
  case QtCamWhiteBalance::Tungsten:
  case QtCamWhiteBalance::Flourescent:
    return (QtCamWhiteBalance::WhiteBalanceMode)val;

  default:
    return QtCamWhiteBalance::Auto;
  }
}

bool QtCamWhiteBalance::setValue(const QtCamWhiteBalance::WhiteBalanceMode& mode) {
  return d_ptr->setIntValue(mode, false);
}
