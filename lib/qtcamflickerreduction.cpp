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

#include "qtcamflickerreduction.h"
#include "qtcamcapability_p.h"

QtCamFlickerReduction::QtCamFlickerReduction(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::FlickerReduction,
					     "flicker-mode"), parent) {

}

QtCamFlickerReduction::~QtCamFlickerReduction() {

}

QtCamFlickerReduction::FlickerReductionMode QtCamFlickerReduction::value() {
  int val = 0;
  if (!d_ptr->intValue(&val)) {
    return QtCamFlickerReduction::Auto;
  }

  switch (val) {
  case QtCamFlickerReduction::Off:
  case QtCamFlickerReduction::FiftyHz:
  case QtCamFlickerReduction::SixtyHz:
    return (QtCamFlickerReduction::FlickerReductionMode)val;

  default:
    return QtCamFlickerReduction::Auto;
  }
}

bool QtCamFlickerReduction::setValue(const QtCamFlickerReduction::FlickerReductionMode& mode) {
  return d_ptr->setIntValue(mode, false);
}
