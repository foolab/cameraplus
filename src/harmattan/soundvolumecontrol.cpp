// -*- c++ -*-

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

#include "soundvolumecontrol.h"
#include <contextsubscriber/contextproperty.h>

#define AUDIO_ROUTE_PROPERTY         "/com/nokia/policy/audio_route"
#define AUDIO_ROUTE_SPEAKERS         "ihf"

SoundVolumeControl::SoundVolumeControl(QObject *parent) :
  QObject(parent),
  m_audioRoute(new ContextProperty(AUDIO_ROUTE_PROPERTY, this)) {

  QObject::connect(m_audioRoute, SIGNAL(valueChanged()), this, SLOT(audioConnectionChanged()));
  m_audioRoute->waitForSubscription(true);
  audioConnectionChanged();
}

SoundVolumeControl::~SoundVolumeControl() {

}

bool SoundVolumeControl::fullVolume() {
  return m_fullVolume;
}

void SoundVolumeControl::audioConnectionChanged() {
  m_fullVolume = m_audioRoute->value().toString() == QLatin1String(AUDIO_ROUTE_SPEAKERS);

  emit fullVolumeChanges();
}
