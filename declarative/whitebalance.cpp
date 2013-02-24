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

#include "whitebalance.h"

WhiteBalance::WhiteBalance(QtCamDevice *dev, QObject *parent) :
  QObject(parent),
  m_wb(new QtCamWhiteBalance(dev, this)) {

  QObject::connect(m_wb, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
}

WhiteBalance::~WhiteBalance() {
  delete m_wb; m_wb = 0;
}

WhiteBalance::WhiteBalanceMode WhiteBalance::value() {
  return (WhiteBalanceMode)m_wb->value();
}

void WhiteBalance::setValue(const WhiteBalance::WhiteBalanceMode& mode) {
  m_wb->setValue((QtCamWhiteBalance::WhiteBalanceMode)mode);
}
