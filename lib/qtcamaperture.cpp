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

#include "qtcamaperture.h"
#include "qtcamcapability_p.h"

QtCamAperture::QtCamAperture(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::Aperture, "aperture"),
		  parent) {

}

QtCamAperture::~QtCamAperture() {

}

unsigned int QtCamAperture::value() {
  unsigned int val = defaultValue();

  d_ptr->uintValue(&val);

  return val;
}

bool QtCamAperture::setValue(unsigned int val) {
  return d_ptr->setUintValue(val);
}

unsigned int QtCamAperture::minimumValue() {
  GParamSpec *p = d_ptr->paramSpec();

  if (p && G_IS_PARAM_SPEC_UINT(p)) {
    return G_PARAM_SPEC_UINT(p)->minimum;
  }

  return 0;
}

unsigned int QtCamAperture::maximumValue() {
  GParamSpec *p = d_ptr->paramSpec();

  if (p && G_IS_PARAM_SPEC_UINT(p)) {
    return G_PARAM_SPEC_UINT(p)->maximum;
  }

  return 0;
}

unsigned int QtCamAperture::defaultValue() {
  GParamSpec *p = d_ptr->paramSpec();

  if (p && G_IS_PARAM_SPEC_UINT(p)) {
    return G_PARAM_SPEC_UINT(p)->default_value;
  }

  return 0;
}
