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

#include "displaystate.h"
#include <qmsystem2/qmdisplaystate.h>
#include <QTimer>
#include <QDebug>

DisplayState::DisplayState(QObject *parent) :
  QObject(parent), m_state(new MeeGo::QmDisplayState(this)), m_timer(new QTimer(this)) {

  m_timer->setSingleShot(false);
  m_timer->setInterval(50 * 1000);

  QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

DisplayState::~DisplayState() {
  setInhibitDim(false);
}

bool DisplayState::isDimInhibited() const {
  return m_timer->isActive();
}

void DisplayState::setInhibitDim(bool inhibit) {
  if (m_timer->isActive() == inhibit) {
    return;
  }

  if (!inhibit) {
    if (!m_state->cancelBlankingPause()) {
      qWarning() << "Failed to cancel display dimming!";
    }

    m_timer->stop();
  }
  else {
    if (!m_state->setBlankingPause()) {
      qWarning() << "Failed to inhibit display dimming!";
      return;
    }

    m_timer->start();
  }

  emit inhibitDimChanged();
}

void DisplayState::timeout() {
  if (!m_state->setBlankingPause()) {
    qWarning() << "Failed to inhibit display dimming!";
  }
}
