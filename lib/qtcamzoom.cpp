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

#include "qtcamzoom.h"
#include "qtcamcapability_p.h"
#include "qtcamdevice_p.h"

class QtCamZoomPrivate : public QtCamCapabilityPrivate {
public:
  QtCamZoomPrivate(QtCamDevice *d, QtCamZoom *q) :
    QtCamCapabilityPrivate(d, QtCamCapability::Zoom), q_ptr(q), binHandler(0) {

  }

  ~QtCamZoomPrivate() {
    if (binHandler) {
      g_signal_handler_disconnect(bin, binHandler);
    }
  }

  void init() {
    if (bin) {
      binHandler = g_signal_connect(bin, "notify", G_CALLBACK(camera_bin_notify), this);
    }
  }

  static void camera_bin_notify(GObject *gobject, GParamSpec *pspec, QtCamZoomPrivate *d) {
    Q_UNUSED(gobject);

    QLatin1String name(pspec->name);
    if (name == QLatin1String("max-zoom")) {
      QMetaObject::invokeMethod(d->q_ptr, "maximumValueChanged", Qt::QueuedConnection);
    }
    else if (name == QLatin1String("zoom")) {
      QMetaObject::invokeMethod(d->q_ptr, "valueChanged", Qt::QueuedConnection);
    }
  }

  qreal zoom() {
    if (!bin) {
      return 1.0;
    }

    gfloat v = 1.0;

    g_object_get(bin, "zoom", &v, NULL);

    return v;
  }

  qreal maxZoom() {
    if (!bin) {
      return 1.0;
    }

    gfloat v = 1.0;

    g_object_get(bin, "max-zoom", &v, NULL);

    return v;
  }

  bool setZoom(qreal zoom) {
    if (!bin) {
      return false;
    }

    if (qFuzzyCompare(QtCamZoomPrivate::zoom(), zoom)) {
	return false;
      }

    g_object_set(bin, "zoom", zoom, NULL);

    return true;
  }

  QtCamZoom *q_ptr;
  gulong binHandler;
};

QtCamZoom::QtCamZoom(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamZoomPrivate(dev, this), parent) {

  ((QtCamZoomPrivate *) d_ptr)->init();
}

QtCamZoom::~QtCamZoom() {

}

qreal QtCamZoom::value() {
  return ((QtCamZoomPrivate *) d_ptr)->zoom();
}

bool QtCamZoom::setValue(qreal zoom) {
  if (((QtCamZoomPrivate *) d_ptr)->setZoom(zoom)) {
    emit valueChanged();
    return true;
  }

  return false;
}

qreal QtCamZoom::minimumValue() {
  return 1.0;
}

qreal QtCamZoom::maximumValue() {
  return ((QtCamZoomPrivate *) d_ptr)->maxZoom();
}
