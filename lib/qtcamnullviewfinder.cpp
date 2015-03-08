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

#include "qtcamnullviewfinder.h"
#include <gst/gst.h>

QtCamNullViewfinder::QtCamNullViewfinder(QObject *parent) :
  QObject(parent),
  m_sink(0) {

}

QtCamNullViewfinder::~QtCamNullViewfinder() {

}

GstElement *QtCamNullViewfinder::sinkElement() {
  if (!m_sink) {
    m_sink = gst_element_factory_make ("fakesink", "QtCamNullViewfinder");
    g_object_add_toggle_ref(G_OBJECT(m_sink), (GToggleNotify)sink_notify, this);
  }

  return m_sink;
}

void QtCamNullViewfinder::start() {

}

void QtCamNullViewfinder::stop() {

}

QRectF QtCamNullViewfinder::renderArea() const {
  return QRectF();
}

QSizeF QtCamNullViewfinder::videoResolution() const {
  return QSizeF();
}

void QtCamNullViewfinder::sink_notify(QtCamNullViewfinder *q,
				      GObject *object, gboolean is_last_ref) {

  Q_UNUSED(object);

  if (is_last_ref) {
    g_object_remove_toggle_ref(G_OBJECT(q->m_sink), (GToggleNotify)sink_notify, q);
    q->m_sink = 0;
  }
}
