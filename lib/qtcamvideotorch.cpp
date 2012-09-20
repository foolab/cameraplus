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

#include "qtcamvideotorch.h"
#include "qtcamdevice.h"
#include "qtcamdevice_p.h"
#include <QPointer>

class QtCamVideoTorchPrivate {
public:
  static void torch_notify(GObject *gobject, GParamSpec *pspec, QtCamVideoTorch *q) {
    Q_UNUSED(gobject);
    Q_UNUSED(pspec);

    QMetaObject::invokeMethod(q, "stateChanged", Qt::QueuedConnection);
  }

  QPointer<QtCamDevice> dev;
  gulong handler;
};

QtCamVideoTorch::QtCamVideoTorch(QtCamDevice *dev, QObject *parent) :
  QObject(parent), d_ptr(new QtCamVideoTorchPrivate) {

  d_ptr->dev = dev;
  d_ptr->handler = 0;

  if (d_ptr->dev->d_ptr->videoSource) {
    d_ptr->handler = g_signal_connect(d_ptr->dev->d_ptr->videoSource,
				      "notify::video-torch",
				      G_CALLBACK(QtCamVideoTorchPrivate::torch_notify), this);
  }
}

QtCamVideoTorch::~QtCamVideoTorch() {
  if (d_ptr->dev && d_ptr->handler) {
    g_signal_handler_disconnect(d_ptr->dev->d_ptr->videoSource, d_ptr->handler);
  }

  delete d_ptr; d_ptr = 0;
}

void QtCamVideoTorch::setOn(bool on) {
  gboolean val = on ? TRUE : FALSE;

  if (d_ptr->dev->d_ptr->videoSource) {
    g_object_set(d_ptr->dev->d_ptr->videoSource, "video-torch", val, NULL);
  }
}

bool QtCamVideoTorch::isOn() const {
  gboolean val = FALSE;

  if (d_ptr->dev->d_ptr->videoSource) {
    g_object_get(d_ptr->dev->d_ptr->videoSource, "video-torch", &val, NULL);
  }

  return (val == TRUE);
}
