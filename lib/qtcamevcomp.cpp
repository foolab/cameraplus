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

#include "qtcamevcomp.h"
#include "qtcamcapability_p.h"
#include <QDebug>

QtCamEvComp::QtCamEvComp(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::EvComp, "ev-compensation"),
		  parent) {

}

QtCamEvComp::~QtCamEvComp() {

}

qreal QtCamEvComp::value() {
  qreal val = defaultValue();

  d_ptr->floatValue(&val);

  return val;
}

bool QtCamEvComp::setValue(qreal val) {
  return d_ptr->setFloatValue(val);
}

qreal QtCamEvComp::minimumValue() {
  GParamSpec *p = d_ptr->paramSpec();

  if (p && G_IS_PARAM_SPEC_FLOAT(p)) {
    return G_PARAM_SPEC_FLOAT(p)->minimum;
  }

  return 0;
}

qreal QtCamEvComp::maximumValue() {
  GParamSpec *p = d_ptr->paramSpec();

  if (p && G_IS_PARAM_SPEC_FLOAT(p)) {
    return G_PARAM_SPEC_FLOAT(p)->maximum;
  }

  return 0;
}

qreal QtCamEvComp::defaultValue() {
  GParamSpec *p = d_ptr->paramSpec();

  if (p && G_IS_PARAM_SPEC_FLOAT(p)) {
    return G_PARAM_SPEC_FLOAT(p)->default_value;
  }

  return 0;
}
