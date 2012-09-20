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

#include "qtcamexposure.h"
#include "qtcamcapability_p.h"

// TODO: hardcoded
#define EXPOSURE_MIN 0
#define EXPOSURE_MAX 4294967295u

QtCamExposure::QtCamExposure(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::Exposure, "exposure"),
		  parent) {

}

QtCamExposure::~QtCamExposure() {

}

unsigned int QtCamExposure::value() {
  unsigned int val = 0;

  if (!d_ptr->uintValue(&val)) {
    return 0;
  }

  return val;
}

bool QtCamExposure::setValue(unsigned int val) {
  return d_ptr->setUintValue(val);
}

unsigned int QtCamExposure::minimumValue() {
  return EXPOSURE_MIN;
}

unsigned int QtCamExposure::maximumValue() {
  return EXPOSURE_MAX;
}
