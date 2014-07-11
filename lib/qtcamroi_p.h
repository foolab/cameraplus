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

#ifndef QT_CAM_ROI_P_H
#define QT_CAM_ROI_P_H

#include "qtcamdevice.h"
#include "qtcamdevice_p.h"
#include "qtcamgstmessagehandler.h"
#include "qtcamgstmessagelistener.h"
#include "qtcamroi.h"
#include <QDebug>
#include <QPointer>
#include <cmath>

class QtCamRoiPrivate : public QObject {
  Q_OBJECT

public:
  QtCamRoiPrivate(QtCamDevice *device, QtCamRoi *q, QObject *parent = 0) :
    QObject(parent),
    q_ptr(q),
    dev(device),
    roi(0),
    enabled(false),
    msg(0) {

  }

  ~QtCamRoiPrivate() {
    if (roi) {
      gst_object_unref(roi);
    }

    if (handler) {
      dev->listener()->removeSyncHandler(handler);

      delete handler.data();
    }

    GstMessage *message = msg.fetchAndStoreOrdered(0);
    if (message) {
      gst_message_unref(message);
      message = 0;
    }
  }

  bool sendEventToSource(GstEvent *event) {
    if (!dev->d_ptr->videoSource) {
      qWarning() << "No video source";
      gst_event_unref(event);
      return false;
    }

    return gst_element_send_event(dev->d_ptr->videoSource, event) == TRUE ? true : false;
  }

  void init() {
    roi = dev->d_ptr->findByFactory(dev->config()->roiElement().toUtf8().constData());
  }

  void installHandler() {
    if (dev->listener()) {
      handler = new QtCamGstMessageHandler(dev->config()->roiMessage(), this);
      dev->listener()->addSyncHandler(handler);
      QObject::connect(handler, SIGNAL(messageSent(GstMessage *)),
		       this, SLOT(handleMessage(GstMessage *)), Qt::DirectConnection);
      QObject::connect(dev->listener(), SIGNAL(starting()),
		       this, SLOT(recheck()));
    }
  }

  int primaryRegion(const QRectF& area, const QList<QRectF>& rects) {
    int index = -1;
    qreal distance = 0.0;
    QPointF center = area.center();

    for (int x = 0; x < rects.size(); x++) {
      const QRectF& rect = rects.at(x);

      QPointF val = rect.center() - center;

      qreal newDistance = sqrt(pow(val.x(), 2) + pow(val.y(), 2));
      if (index == -1 || newDistance < distance) {
	index = x;
	distance = newDistance;
      }
    }

    return index;
  }

public slots:
  void handleMessage(GstMessage *message) {
    gst_message_ref(message);

    GstMessage *oldMessage = msg.fetchAndStoreOrdered(message);

    if (oldMessage) {
      gst_message_unref(oldMessage);
    }

    QMetaObject::invokeMethod(this, "processMessage", Qt::QueuedConnection);
  }

private slots:
  void recheck() {
    if (roi) {
      gst_object_unref(roi);
    }

    init();

    if (!roi) {
      qWarning() << "Cannot find element" << dev->config()->roiElement();
    }

    q_ptr->setEnabled(enabled);

    emit q_ptr->reset();
  }

  void processMessage() {
    GstMessage *message = msg.fetchAndStoreOrdered(0);
    if (!message) {
      return;
    }

    QList<QRectF> rects;
    unsigned width = 0, height = 0;

    const GstStructure *s = gst_message_get_structure(message);
    if (!gst_structure_get_uint(s, "frame-width", &width) ||
	!gst_structure_get_uint(s, "frame-height", &height)) {

      qWarning() << "Failed to obtain frame dimensions for ROI message";
      gst_message_unref(message);
      return;
    }

    const GValue *regions = gst_structure_get_value(s, "regions");
    if (!regions) {
      qWarning() << "No regions in ROI message";
      gst_message_unref(message);
      return;
    }

    guint size = gst_value_list_get_size(regions);
    for (unsigned i = 0; i < size; i++) {
      const GValue *region = gst_value_list_get_value(regions, i);
      const GstStructure *structure = gst_value_get_structure(region);

      unsigned x = 0, y = 0, w = 0, h = 0;

      gst_structure_get_uint(structure, "region-x", &x);
      gst_structure_get_uint(structure, "region-y", &y);
      gst_structure_get_uint(structure, "region-w", &w);
      gst_structure_get_uint(structure, "region-h", &h);

      QRectF rect((qreal)x/width, (qreal)y/height, (qreal)w/width, (qreal)h/height);

      rects << rect;
    }

    int index = primaryRegion(QRectF(0, 0, 1, 1), rects);

    QList<QRectF> rest(rects);
    QRectF primary = index == -1 ? QRectF() : rest.takeAt(index);

    emit q_ptr->regionsOfInterestUpdated(rects, primary, rest);

    gst_message_unref(message);
  }

public:
  QtCamRoi *q_ptr;
  QtCamDevice *dev;
  GstElement *roi;
  bool enabled;
  QPointer<QtCamGstMessageHandler> handler;
  QAtomicPointer<GstMessage> msg;
};

#endif /* QT_CAM_ROI_P_H */
