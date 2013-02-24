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

#include "qtcamfocus.h"
#include "qtcamcapability_p.h"

QtCamFocus::QtCamFocus(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::Focus, "focus-mode"), parent) {

}

QtCamFocus::~QtCamFocus() {

}

QtCamFocus::FocusMode QtCamFocus::value() {
  int val = 0;
  if (!d_ptr->intValue(&val)) {
    return QtCamFocus::Auto;
  }

  switch (val) {
  case QtCamFocus::Macro:
  case QtCamFocus::Portrait:
  case QtCamFocus::Infinity:
  case QtCamFocus::Hyperfocal:
  case QtCamFocus::Extended:
  case QtCamFocus::ContinuousNormal:
  case QtCamFocus::ContinuousExtended:
  case QtCamFocus::Sport:
    return (QtCamFocus::FocusMode)val;

  default:
    return QtCamFocus::Auto;
  }
}

bool QtCamFocus::setValue(const FocusMode& mode) {
  return d_ptr->setIntValue(mode, false);
}
