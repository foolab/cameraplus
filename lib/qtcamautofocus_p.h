// -*- c++ -*-

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

#ifndef QT_CAM_AUTO_FOCUS_P_H
#define QT_CAM_AUTO_FOCUS_P_H

#include "qtcamautofocus.h"
#include "qtcamgstreamermessagehandler.h"
#include "qtcamgstreamermessagelistener.h"
#include "qtcamdevice.h"
#include "qtcamdevice_p.h"

#include <QDebug>

#ifndef GST_USE_UNSTABLE_API
#define GST_USE_UNSTABLE_API
#endif /* GST_USE_UNSTABLE_API */
#include <gst/interfaces/photography.h>

class QtCamAutoFocusPrivate : public QObject {
  Q_OBJECT

public:
  QtCamAutoFocusPrivate(QtCamDevice *device, QtCamAutoFocus *q, QObject *parent = 0) :
    QObject(parent),
    dev(device),
    q_ptr(q),
    status(QtCamAutoFocus::None),
    cafStatus(QtCamAutoFocus::None) {

    handler = new QtCamGStreamerMessageHandler(GST_PHOTOGRAPHY_AUTOFOCUS_DONE, this);

    QObject::connect(handler, SIGNAL(messageSent(GstMessage *)),
		     this, SLOT(handleMessage(GstMessage *)));

    dev->listener()->addHandler(handler);

    cafHandler = new QtCamGStreamerMessageHandler("caf-update", this);

    QObject::connect(cafHandler, SIGNAL(messageSent(GstMessage *)),
		     this, SLOT(handleCafMessage(GstMessage *)));

    dev->listener()->addHandler(cafHandler);
  }

  ~QtCamAutoFocusPrivate() {
    dev->listener()->removeHandler(handler);
    delete handler; handler = 0;
    dev = 0;
    q_ptr = 0;
  }

  bool setEnabled(bool enabled) {
    if (!dev->d_ptr->videoSource) {
      return false;
    }

    if (!GST_IS_PHOTOGRAPHY(dev->d_ptr->videoSource)) {
      return false;
    }

    GstPhotography *photo = GST_PHOTOGRAPHY(dev->d_ptr->videoSource);
    if (!photo) {
      return false;
    }

    gst_photography_set_autofocus(photo, enabled ? TRUE : FALSE);

    if (enabled) {
      status = QtCamAutoFocus::Running;
      emit q_ptr->statusChanged();
    }
    else {
      status = QtCamAutoFocus::None;
      emit q_ptr->statusChanged();
    }

    return true;
  }

  bool setStatus(QtCamAutoFocus::Status *status, GstMessage *message) {
    const GstStructure *s = gst_message_get_structure(message);
    int st = GST_PHOTOGRAPHY_FOCUS_STATUS_NONE;

    if (gst_structure_get_int(s, "status", &st)) {
      if (*status != st) {
	*status = (QtCamAutoFocus::Status) st;
	return true;
      }
    }

    return false;
  }

public slots:
  void handleMessage(GstMessage *message) {
    if (setStatus(&status, message)) {
      // TODO: focus-window-rows, focus-window-columns, focus-windows and the rest
      emit q_ptr->statusChanged();
    }
  }

  void handleCafMessage(GstMessage *message) {
    if (setStatus(&cafStatus, message)) {
      emit q_ptr->cafStatusChanged();
    }
  }

public:
  QtCamDevice *dev;
  QtCamAutoFocus *q_ptr;

  QtCamAutoFocus::Status status;
  QtCamAutoFocus::Status cafStatus;

  QtCamGStreamerMessageHandler *handler;
  QtCamGStreamerMessageHandler *cafHandler;
};

#endif /* QT_CAM_AUTO_FOCUS_P_H */
