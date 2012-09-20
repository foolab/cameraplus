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

#include "qtcammute.h"
#include "qtcamdevice.h"
#include "qtcamdevice_p.h"
#include <QPointer>

class QtCamMutePrivate {
public:
  static void mute_notify(GObject *gobject, GParamSpec *pspec, QtCamMute *q) {
    Q_UNUSED(gobject);
    Q_UNUSED(pspec);

    QMetaObject::invokeMethod(q, "stateChanged", Qt::QueuedConnection);
  }

  QPointer<QtCamDevice> dev;
  gulong handler;
};

QtCamMute::QtCamMute(QtCamDevice *dev, QObject *parent) :
  QObject(parent), d_ptr(new QtCamMutePrivate) {

  d_ptr->dev = dev;
  d_ptr->handler = 0;

  if (d_ptr->dev->d_ptr->cameraBin) {
    d_ptr->handler = g_signal_connect(d_ptr->dev->d_ptr->cameraBin,
				      "notify::mute",
				      G_CALLBACK(QtCamMutePrivate::mute_notify), this);
  }
}

QtCamMute::~QtCamMute() {
  if (d_ptr->dev && d_ptr->handler) {
    g_signal_handler_disconnect(d_ptr->dev->d_ptr->cameraBin, d_ptr->handler);
  }

  delete d_ptr; d_ptr = 0;
}

void QtCamMute::setEnabled(bool enabled) {
  gboolean val = enabled ? TRUE : FALSE;

  if (d_ptr->dev->d_ptr->cameraBin) {
    g_object_set(d_ptr->dev->d_ptr->cameraBin, "mute", val, NULL);
  }
}

bool QtCamMute::isEnabled() const {
  gboolean val = FALSE;

  if (d_ptr->dev->d_ptr->cameraBin) {
    g_object_get(d_ptr->dev->d_ptr->cameraBin, "mute", &val, NULL);
  }

  return (val == TRUE);
}
