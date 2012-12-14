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

#include "qtcamautofocus.h"
#include "qtcamautofocus_p.h"

QtCamAutoFocus::QtCamAutoFocus(QtCamDevice *dev, QObject *parent) :
  QObject(parent),
  d_ptr(new QtCamAutoFocusPrivate(dev, this, this)) {

}

QtCamAutoFocus::~QtCamAutoFocus() {
  delete d_ptr; d_ptr = 0;
}

QtCamAutoFocus::Status QtCamAutoFocus::status() {
  return d_ptr->status;
}

QtCamAutoFocus::Status QtCamAutoFocus::cafStatus() {
  return d_ptr->cafStatus;
}

bool QtCamAutoFocus::startAutoFocus() {
  return d_ptr->setEnabled(true);
}

bool QtCamAutoFocus::stopAutoFocus() {
  return d_ptr->setEnabled(false);
}

bool QtCamAutoFocus::canFocus(const QtCamScene::SceneMode& mode) {
  if (mode == QtCamScene::Landscape || mode == QtCamScene::Sport) {
    return false;
  }

  return true;
}
