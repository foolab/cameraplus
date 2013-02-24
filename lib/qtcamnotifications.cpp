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

#include "qtcamnotifications.h"
#include "qtcamnotifications_p.h"
#include "qtcamdevice.h"
#ifndef GST_USE_UNSTABLE_API
#define GST_USE_UNSTABLE_API
#endif /* GST_USE_UNSTABLE_API */
#include <gst/interfaces/photography.h>

QtCamNotifications::QtCamNotifications(QtCamDevice *dev, QObject *parent) :
  QObject(parent), d_ptr(new QtCamNotificationsPrivate) {

  d_ptr->q_ptr = this;
  d_ptr->dev = dev;

  d_ptr->listener = dev->listener();
  d_ptr->imageStart = new QtCamGStreamerMessageHandler("photo-capture-start", this);
  d_ptr->imageEnd = new QtCamGStreamerMessageHandler("photo-capture-end", this);
  d_ptr->videoDone = new QtCamGStreamerMessageHandler("video-done", this);
  d_ptr->af = new QtCamGStreamerMessageHandler(GST_PHOTOGRAPHY_AUTOFOCUS_DONE, this);

  if (d_ptr->listener) {
    d_ptr->listener->addSyncHandler(d_ptr->imageStart);
    d_ptr->listener->addHandler(d_ptr->imageEnd);
    d_ptr->listener->addHandler(d_ptr->videoDone);
    d_ptr->listener->addHandler(d_ptr->af);
  }

  QObject::connect(d_ptr->imageStart, SIGNAL(messageSent(GstMessage *)),
		   this, SIGNAL(imageCaptureStarted()), Qt::DirectConnection);

  QObject::connect(d_ptr->imageEnd, SIGNAL(messageSent(GstMessage *)),
		   this, SIGNAL(imageCaptureEnded()), Qt::DirectConnection);

  QObject::connect(d_ptr->videoDone, SIGNAL(messageSent(GstMessage *)),
		   this, SIGNAL(videoRecordingEnded()), Qt::DirectConnection);

  QObject::connect(d_ptr->af, SIGNAL(messageSent(GstMessage *)),
		   this, SLOT(autoFocusStatusChanged(GstMessage *)));
}

QtCamNotifications::~QtCamNotifications() {
  if (d_ptr->listener) {
    d_ptr->listener->removeSyncHandler(d_ptr->imageStart);
    d_ptr->listener->removeHandler(d_ptr->imageEnd);
    d_ptr->listener->removeHandler(d_ptr->videoDone);
    d_ptr->listener->removeHandler(d_ptr->af);
  }

  delete d_ptr->imageStart.data();
  delete d_ptr->imageEnd.data();
  delete d_ptr->videoDone.data();
  delete d_ptr->af.data();

  delete d_ptr; d_ptr = 0;
}

#include "moc_qtcamnotifications.cpp"
