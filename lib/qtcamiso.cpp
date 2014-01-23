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

#include "qtcamiso.h"
#include "qtcamcapability_p.h"

QtCamIso::QtCamIso(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::IsoSpeed, "iso-speed"),
		  parent) {

}

QtCamIso::~QtCamIso() {

}

unsigned int QtCamIso::value() {
  unsigned int val = defaultValue();

  d_ptr->uintValue(&val);

  return val;
}

bool QtCamIso::setValue(unsigned int val) {
  return d_ptr->setUintValue(val);
}

unsigned int QtCamIso::minimumValue() {
  GParamSpec *p = d_ptr->paramSpec();

  if (p && G_IS_PARAM_SPEC_UINT(p)) {
    return G_PARAM_SPEC_UINT(p)->minimum;
  }

  return 0;
}

unsigned int QtCamIso::maximumValue() {
  GParamSpec *p = d_ptr->paramSpec();

  if (p && G_IS_PARAM_SPEC_UINT(p)) {
    return G_PARAM_SPEC_UINT(p)->maximum;
  }

  return 0;
}

unsigned int QtCamIso::defaultValue() {
  GParamSpec *p = d_ptr->paramSpec();

  if (p && G_IS_PARAM_SPEC_UINT(p)) {
    return G_PARAM_SPEC_UINT(p)->default_value;
  }

  return 0;
}
