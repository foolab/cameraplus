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

#ifndef QT_CAM_FLASH_P_H
#define QT_CAM_FLASH_P_H

#include <QObject>
#include "qtcamcapability_p.h"
#include "qtcamdevice.h"
#include "qtcamgstreamermessagelistener.h"
#include "qtcamgstreamermessagehandler.h"
#include <QDebug>

// subdevsrc ./gst-libs/gst/camera/gstcamerasrc2.h
#define FLASH_STATUS_READY        0
#define FLASH_STATUS_NOT_READY    1

class QtCamFlashPrivate : public QObject, public QtCamCapabilityPrivate {
  Q_OBJECT

public:
  QtCamFlashPrivate(QtCamDevice *dev, QObject *parent = 0) :
    QObject(parent),
    QtCamCapabilityPrivate(dev, QtCamCapability::Flash, "flash-mode"),
    ready(true) {

  }

  ~QtCamFlashPrivate() {

  }

  void init() {
    QtCamGStreamerMessageListener *listener = dev->listener();
    if (!listener) {
      qWarning() << "Failed to get device listener. flash ready status will not be available";
      return;
    }

    QtCamGStreamerMessageHandler *handler = new QtCamGStreamerMessageHandler("flash-status", this);
    QObject::connect(handler, SIGNAL(messageSent(GstMessage *)),
		     this, SLOT(messageSent(GstMessage *)));
    listener->addHandler(handler);
  }

  bool isReady() const {
    return ready;
  }

signals:
  void flashReadyChanged();

private slots:
  void messageSent(GstMessage *message) {
    const GstStructure *s = gst_message_get_structure(message);
    if (QLatin1String("flash-status") != QLatin1String(gst_structure_get_name(s))) {
      return;
    }

    int status = FLASH_STATUS_NOT_READY;
    if (!gst_structure_get_int(s, "status", &status)) {
      return;
    }

    bool newStatus = (status == FLASH_STATUS_READY);

    if (ready != newStatus) {
      ready = newStatus;
      emit flashReadyChanged();
    }
  }

private:
  bool ready;
};

#endif /* QT_CAM_FLASH_P_H */
