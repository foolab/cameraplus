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

#include "qtcamflash.h"
#include "qtcamcapability_p.h"
#include "qtcamflash_p.h"

QtCamFlash::QtCamFlash(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamFlashPrivate(dev, parent), parent) {

  QtCamFlashPrivate *pvt = dynamic_cast<QtCamFlashPrivate *>(d_ptr);

  // We get a weird crash if we pass this as parent to QtCamFlashPrivate
  pvt->setParent(this);

  QObject::connect(pvt, SIGNAL(flashReadyChanged()), this, SIGNAL(flashReadyChanged()));

  pvt->init();
}

QtCamFlash::~QtCamFlash() {

}

QtCamFlash::FlashMode QtCamFlash::value() {
  int val = 0;
  if (!d_ptr->intValue(&val)) {
    return QtCamFlash::Auto;
  }

  switch (val) {
  case QtCamFlash::On:
  case QtCamFlash::Off:
  case QtCamFlash::FillIn:
  case QtCamFlash::RedEye:
    return (QtCamFlash::FlashMode)val;

  default:
    return QtCamFlash::Auto;
  }
}

bool QtCamFlash::setValue(const QtCamFlash::FlashMode& mode) {
  return d_ptr->setIntValue(mode, false);
}

bool QtCamFlash::isReady() const {
  return dynamic_cast<QtCamFlashPrivate *>(d_ptr)->isReady();
}
