// -*- c++ -*-

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

  bool intValue(int *val) {
    if (!src) {
      return false;
    }

    g_object_get(src, prop.toAscii().data(), val, NULL);

    return true;
  }

  bool setIntValue(int val) {
    int old = 0;

    if (!src) {
      return false;
    }

    intValue(&old);

    if (old == val) {
      return true;
    }

    g_object_set(src, prop.toAscii().data(), val, NULL);

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
    qreal old = 0;

    if (!src) {
      return false;
    }

    floatValue(&old);

    if (qFuzzyCompare(old, val)) {
      return true;
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
