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

#ifndef QT_CAM_VIEWFINDER_BUFFER_LISTENER_P_H
#define QT_CAM_VIEWFINDER_BUFFER_LISTENER_P_H

#include <QMutex>
#include <gst/gst.h>

class QtCamViewfinderBufferListenerPrivate {
public:
  QtCamViewfinderBufferListenerPrivate(QtCamDevicePrivate *d);
  ~QtCamViewfinderBufferListenerPrivate();

  void setSink(GstElement *sink);

  void addHandler(QtCamViewfinderBufferHandler *handler);
  void removeHandler(QtCamViewfinderBufferHandler *handler);

private:
  void addBufferProbe();
  void removeBufferProbe();

  QtCamDevicePrivate *dev;
  GstElement *sink;
  GstPad *pad;
  gulong probe_id;
  QMutex mutex;
  QList<QtCamViewfinderBufferHandler *> handlers;

#if GST_CHECK_VERSION(1,0,0)
  static GstPadProbeReturn buffer_probe(GstPad *pad, GstPadProbeInfo *info,
					gpointer user_data);
#else
  static gboolean buffer_probe(GstPad *pad, GstBuffer *buffer,
			       gpointer user_data);
#endif
};

#endif /* QT_CAM_VIEWFINDER_BUFFER_LISTENER_P_H */
