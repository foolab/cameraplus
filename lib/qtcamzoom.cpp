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

#include "qtcamzoom.h"
#include "qtcamcapability_p.h"
#include "qtcamdevice_p.h"

class QtCamZoomPrivate : public QtCamCapabilityPrivate {
public:
  QtCamZoomPrivate(QtCamDevice *d, QtCamZoom *q) :
    QtCamCapabilityPrivate(d, QtCamCapability::Zoom, "zoom"), q_ptr(q), handler(0) {
  }

  ~QtCamZoomPrivate() {
    if (handler > 0) {
      g_signal_handler_disconnect(src, handler);
    }
  }

  static void max_zoom_notify(GObject *gobject, GParamSpec *pspec, QtCamZoomPrivate *d) {
    Q_UNUSED(gobject);
    Q_UNUSED(pspec);

    QMetaObject::invokeMethod(d->q_ptr, "maximumValueChanged", Qt::QueuedConnection);
  }

  QtCamZoom *q_ptr;
  gulong handler;
};

QtCamZoom::QtCamZoom(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamZoomPrivate(dev, this), parent) {

  // QtCamCapabilityPrivate::src is initialized in QtCamCapability itself
  // not in the private class so we have to connect to the signal from here.
  QtCamZoomPrivate *d = dynamic_cast<QtCamZoomPrivate *>(d_ptr);

  d->handler =
    g_signal_connect(d->src, "notify::max-zoom",
		     G_CALLBACK(QtCamZoomPrivate::max_zoom_notify), d);
}

QtCamZoom::~QtCamZoom() {

}

qreal QtCamZoom::value() {
  qreal val = defaultValue();

  d_ptr->floatValue(&val);

  return val;
}

bool QtCamZoom::setValue(qreal zoom) {
  return d_ptr->setFloatValue(zoom);
}

qreal QtCamZoom::minimumValue() {
  GParamSpec *p = d_ptr->paramSpec();

  if (p && G_IS_PARAM_SPEC_FLOAT(p)) {
    return G_PARAM_SPEC_FLOAT(p)->minimum;
  }

  return 1.0;
}

qreal QtCamZoom::maximumValue() {
  gfloat max = 1.0;

  g_object_get (d_ptr->src, "max-zoom", &max, NULL);

  return max;
}

qreal QtCamZoom::defaultValue() {
  GParamSpec *p = d_ptr->paramSpec();

  if (p && G_IS_PARAM_SPEC_FLOAT(p)) {
    return G_PARAM_SPEC_FLOAT(p)->default_value;
  }

  return 1.0;
}
