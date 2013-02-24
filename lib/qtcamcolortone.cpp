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

#include "qtcamcolortone.h"
#include "qtcamcapability_p.h"

QtCamColorTone::QtCamColorTone(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::ColourTone, "colour-tone-mode"),
		  parent) {

}

QtCamColorTone::~QtCamColorTone() {

}

QtCamColorTone::ColorToneMode QtCamColorTone::value() {
  int val = 0;
  if (!d_ptr->intValue(&val)) {
    return QtCamColorTone::Normal;
  }

  switch (val) {
  case QtCamColorTone::Sepia:
  case QtCamColorTone::Negative:
  case QtCamColorTone::GrayScale:
  case QtCamColorTone::Natural:
  case QtCamColorTone::Vivid:
  case QtCamColorTone::ColorSwap:
  case QtCamColorTone::Solarize:
  case QtCamColorTone::OutOfFocus:
  case QtCamColorTone::SkyBlue:
  case QtCamColorTone::GrassGreen:
  case QtCamColorTone::SkinWhite:
    return (QtCamColorTone::ColorToneMode)val;

  default:
    return QtCamColorTone::Normal;
  }
}

bool QtCamColorTone::setValue(const QtCamColorTone::ColorToneMode& mode) {
  return d_ptr->setIntValue(mode, false);
}
