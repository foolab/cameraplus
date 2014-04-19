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

#include "qtcamevcomp.h"
#include "qtcamcapability_p.h"
#include <QDebug>

class QtCamEvCompPrivate : public QtCamCapabilityPrivate {
public:
  QtCamEvCompPrivate(QtCamDevice *d, QtCamEvComp *q) :
    QtCamCapabilityPrivate(d, QtCamCapability::EvComp, "ev-compensation"),
    q_ptr(q),
    minHandler(0),
    maxHandler(0) {

  }

  ~QtCamEvCompPrivate() {
    if (minHandler > 0) {
      g_signal_handler_disconnect(src, minHandler);
    }

    if (maxHandler > 0) {
      g_signal_handler_disconnect(src, maxHandler);
    }
  }

  static void min_ev_notify(GObject *gobject, GParamSpec *pspec, QtCamEvCompPrivate *d) {
    Q_UNUSED(gobject);
    Q_UNUSED(pspec);

    QMetaObject::invokeMethod(d->q_ptr, "minimumValueChanged", Qt::QueuedConnection);
  }

  static void max_ev_notify(GObject *gobject, GParamSpec *pspec, QtCamEvCompPrivate *d) {
    Q_UNUSED(gobject);
    Q_UNUSED(pspec);

    QMetaObject::invokeMethod(d->q_ptr, "maximumValueChanged", Qt::QueuedConnection);
  }

  QtCamEvComp *q_ptr;
  gulong minHandler;
  gulong maxHandler;
};

QtCamEvComp::QtCamEvComp(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamEvCompPrivate(dev, this), parent) {

  QtCamEvCompPrivate *d = dynamic_cast<QtCamEvCompPrivate *>(d_ptr);

  d->minHandler =
    g_signal_connect(d->src, "notify::min-ev-compensation",
		     G_CALLBACK(QtCamEvCompPrivate::min_ev_notify), d);

  d->maxHandler =
    g_signal_connect(d->src, "notify::max-ev-compensation",
		     G_CALLBACK(QtCamEvCompPrivate::max_ev_notify), d);
}

QtCamEvComp::~QtCamEvComp() {

}

qreal QtCamEvComp::value() {
  qreal val = defaultValue();

  d_ptr->floatValue(&val);

  return val;
}

bool QtCamEvComp::setValue(qreal val) {
  return d_ptr->setFloatValue(val);
}

qreal QtCamEvComp::minimumValue() {
  QtCamEvCompPrivate *d = dynamic_cast<QtCamEvCompPrivate *>(d_ptr);
  if (d->minHandler > 0) {
    gfloat val = 0.0;
    g_object_get (d_ptr->src, "min-ev-compensation", &val, NULL);

    return val;
  }

  GParamSpec *p = d_ptr->paramSpec();

  if (p && G_IS_PARAM_SPEC_FLOAT(p)) {
    return G_PARAM_SPEC_FLOAT(p)->minimum;
  }

  return 0;
}

qreal QtCamEvComp::maximumValue() {
  QtCamEvCompPrivate *d = dynamic_cast<QtCamEvCompPrivate *>(d_ptr);
  if (d->maxHandler > 0) {
    gfloat val = 0.0;
    g_object_get (d_ptr->src, "max-ev-compensation", &val, NULL);

    return val;
  }

  GParamSpec *p = d_ptr->paramSpec();

  if (p && G_IS_PARAM_SPEC_FLOAT(p)) {
    return G_PARAM_SPEC_FLOAT(p)->maximum;
  }

  return 0;
}

qreal QtCamEvComp::defaultValue() {
  GParamSpec *p = d_ptr->paramSpec();

  if (p && G_IS_PARAM_SPEC_FLOAT(p)) {
    return G_PARAM_SPEC_FLOAT(p)->default_value;
  }

  return 0;
}
