// -*- c++ -*-

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

#include "phoneprofile.h"
#include <contextproperty.h>

#define PHONE_PROFILE_PROPERTY "Profile.Name"
#define SILENT_PROFILE_NAME    "silent"

PhoneProfile::PhoneProfile(QObject *parent) :
  QObject(parent),
  m_profile(new ContextProperty(PHONE_PROFILE_PROPERTY, this)),
  m_isSilent(false) {

  QObject::connect(m_profile, SIGNAL(valueChanged()), this, SLOT(phoneProfileChanged()));
  m_profile->waitForSubscription(true);
  phoneProfileChanged();
}

PhoneProfile::~PhoneProfile() {

}

bool PhoneProfile::isSilent() {
  return m_isSilent;
}

void PhoneProfile::phoneProfileChanged() {
  bool silent = (m_profile->value().toString() == QLatin1String(SILENT_PROFILE_NAME));

  if (silent != m_isSilent) {
    m_isSilent = silent;
    emit isSilentChanged();
  }
}
