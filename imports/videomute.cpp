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

#include "videomute.h"
#include "qtcamvideomute.h"

VideoMute::VideoMute(QtCamDevice *dev, QObject *parent) :
  QObject(parent),
  m_mute(new QtCamVideoMute(dev, this)) {

  QObject::connect(m_mute, SIGNAL(stateChanged()), this, SIGNAL(stateChanged()));
}

VideoMute::~VideoMute() {
  delete m_mute; m_mute = 0;
}

bool VideoMute::isEnabled() const {
  return m_mute->isEnabled();
}

void VideoMute::setEnabled(bool enabled) {
  m_mute->setEnabled(enabled);
}
