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

#ifndef QT_CAM_NOTIFICATIONS_P_H
#define QT_CAM_NOTIFICATIONS_P_H

#include <QPointer>
#include "qtcamgstmessagehandler.h"
#include "qtcamgstmessagelistener.h"
#include "qtcamnotifications.h"
#ifndef GST_USE_UNSTABLE_API
#define GST_USE_UNSTABLE_API
#endif /* GST_USE_UNSTABLE_API */
#include <gst/interfaces/photography.h>

class QtCamNotificationsPrivate : public QObject {
  Q_OBJECT

public:
  QtCamDevice *dev;
  QPointer<QtCamGstMessageHandler> imageStart;
  QPointer<QtCamGstMessageHandler> imageEnd;

  QPointer<QtCamGstMessageHandler> videoDone;

  QPointer<QtCamGstMessageHandler> af;

  QPointer<QtCamGstMessageListener> listener;

  QtCamNotifications *q_ptr;

public slots:
  void autoFocusStatusChanged(GstMessage *message) {
    if (!message || !gst_message_get_structure(message)) {
      return;
    }

    const GstStructure *s = gst_message_get_structure(message);
    int st = GST_PHOTOGRAPHY_FOCUS_STATUS_NONE;
    if (gst_structure_get_int(s, "status", &st)) {
      if (st == GST_PHOTOGRAPHY_FOCUS_STATUS_SUCCESS) {
	emit q_ptr->autoFocusAcquired();
      } else if (st == GST_PHOTOGRAPHY_FOCUS_STATUS_FAIL) {
	emit q_ptr->autoFocusFailed();
      }
    }
  }
};

#endif /* QT_CAM_NOTIFICATIONS_P_H */
