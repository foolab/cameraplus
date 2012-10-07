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

#ifndef QT_CAM_CAPABILITY_P_H
#define QT_CAM_CAPABILITY_P_H

#include <gst/gst.h>
#include "qtcamdevice.h"
#include "qtcamdevice_p.h"

class QtCamCapabilityPrivate {
public:
  QtCamCapabilityPrivate(QtCamDevice *d, const QtCamCapability::Capability& c, const QString& p) :
    dev(d),
    cap(c),
    prop(p),
    bin(0),
    src(0),
    handler(0),
    q_ptr(0) {

  }

  QtCamCapabilityPrivate(QtCamDevice *d, const QtCamCapability::Capability& c) :
    dev(d),
    cap(c),
    bin(0),
    src(0),
    handler(0),
    q_ptr(0) {

  }

  virtual ~QtCamCapabilityPrivate() {

  }

  GParamSpec *paramSpec() {
    if (!src || prop.isEmpty()) {
      return 0;
    }

    return g_object_class_find_property(G_OBJECT_GET_CLASS(src), prop.toUtf8().constData());
  }

  void startMonitoring() {
    if (src) {
      QString p = QString("notify::%1").arg(prop);
      handler = g_signal_connect(src, p.toAscii().data(), G_CALLBACK(camera_src_notify), this);
    }
  }

  void stopMonitoring() {
    if (src) {
      g_signal_handler_disconnect(src, handler);
    }
  }

  bool uintValue(unsigned int *val) {
    if (!src) {
      return false;
    }

    g_object_get(src, prop.toAscii().data(), val, NULL);

    return true;
  }

  bool setUintValue(unsigned int val) {
    if (!src) {
      return false;
    }

    g_object_set(src, prop.toAscii().data(), val, NULL);

    return true;
  }

  bool intValue(int *val) {
    if (!src) {
      return false;
    }

    g_object_get(src, prop.toAscii().data(), val, NULL);

    return true;
  }

  bool setIntValue(int val, bool force) {
    if (!src) {
      return false;
    }

    if (force) {
      g_object_set(src, prop.toAscii().data(), val, NULL);
      return true;
    }

    int old = 0;
    g_object_get(src, prop.toAscii().data(), &old, NULL);

    if (old != val) {
      g_object_set(src, prop.toAscii().data(), val, NULL);
    }

    return true;
  }

  bool floatValue(qreal *val) {
    if (!src) {
      return false;
    }

    g_object_get(src, prop.toAscii().data(), val, NULL);

    return true;
  }

  bool setFloatValue(qreal val) {
    if (!src) {
      return false;
    }

    g_object_set(src, prop.toAscii().data(), val, NULL);

    return true;
  }

  static void camera_src_notify(GObject *gobject, GParamSpec *pspec, QtCamCapabilityPrivate *d) {
    Q_UNUSED(gobject);
    Q_UNUSED(pspec);

    QMetaObject::invokeMethod(d->q_ptr, "valueChanged", Qt::QueuedConnection);
  }

  QtCamDevice *dev;
  QtCamCapability::Capability cap;
  QString prop;
  GstElement *bin;
  GstElement *src;
  gulong handler;
  QtCamCapability *q_ptr;
};

#endif /* QT_CAM_CAPABILITY_P_H */
