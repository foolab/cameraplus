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

#include "platformquirks.h"
#include <QDeclarativeView>
#include <QApplication>
#include <QEvent>
#include <QX11Info>
#include <X11/Xlib.h>

PlatformQuirks::PlatformQuirks(QObject *parent) :
  QObject(parent),
  m_active(false) {

  qApp->installEventFilter(this);
}

PlatformQuirks::~PlatformQuirks() {

}

bool PlatformQuirks::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::ApplicationActivate) {
    if (!m_active) {
      m_active = true;
      emit windowActiveChanged();
    }
  }
  else if (event->type() == QEvent::ApplicationDeactivate) {
    if (coveredByNetworkOrTermsDialog()) {
      if (!m_active) {
	m_active = true;
	emit windowActiveChanged();
      }
    } else {
      if (m_active) {
	m_active = false;
	emit windowActiveChanged();
      }
    }
  }
  else if (event->type() == QEvent::WindowStateChange) {
    if (!state().testFlag(Qt::WindowMaximized)) {
      if (m_active) {
	m_active = false;
	emit windowActiveChanged();
      }
    }
  }

  return QObject::eventFilter(obj, event);
}

bool PlatformQuirks::isWindowActive() const {
  return m_active;
}

Qt::WindowStates PlatformQuirks::state() {
  QWidgetList widgets = qApp->allWidgets();

  foreach (QWidget *w, widgets) {
    if (QDeclarativeView *v = dynamic_cast <QDeclarativeView *>(w)) {
      return v->windowState();
    }
  }

  return Qt::WindowStates();
}

bool PlatformQuirks::coveredByNetworkOrTermsDialog() {
  int param = 0;
  char *winName = 0;
  Window winFocus;

  Display *d = QX11Info::display();

  XGetInputFocus(d, &winFocus, &param);
  XFetchName(d, winFocus, &winName);

  bool covered = (QLatin1String(winName) == "location-ui" || QLatin1String(winName) == "conndlgs");
  XFree(winName);

  return covered;
}
