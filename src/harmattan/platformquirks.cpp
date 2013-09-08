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

#include "platformquirks.h"
#include <QApplication>
#include <QEvent>
#include <QX11Info>
#include <X11/Xlib.h>

PlatformQuirks::PlatformQuirks(QObject *parent) :
  QObject(parent),
  m_state(false),
  m_check(true) {

  qApp->installEventFilter(this);
}

PlatformQuirks::~PlatformQuirks() {

}

bool PlatformQuirks::isOnForced() {
  if (!m_check) {
    return m_state;
  }

  int param = 0;
  char *winName = 0;
  Window winFocus;

  Display *d = QX11Info::display();

  XGetInputFocus(d, &winFocus, &param);
  XFetchName(d, winFocus, &winName);

  m_state = (QLatin1String(winName) == "location-ui" || QLatin1String(winName) == "conndlgs");
  XFree(winName);

  m_check = false;
  return m_state;
}

bool PlatformQuirks::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::ApplicationActivate) {
    m_check = false;

    if (!m_state) {
      m_state = true;
      emit forceOnChanged();
    }
  }
  else if (event->type() == QEvent::ApplicationDeactivate) {
    m_check = true;
    emit forceOnChanged();
  }

  return QObject::eventFilter(obj, event);
}
