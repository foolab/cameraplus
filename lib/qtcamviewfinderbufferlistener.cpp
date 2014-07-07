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

#include "qtcamviewfinderbufferlistener.h"
#include "qtcamviewfinderbufferlistener_p.h"
#include "qtcamviewfinderbufferhandler.h"
#include <QDebug>

QtCamViewfinderBufferListenerPrivate::QtCamViewfinderBufferListenerPrivate(QtCamDevicePrivate *d) :
  dev(d),
  sink(0),
  pad(0),
  probe_id(0) {

}

QtCamViewfinderBufferListenerPrivate::~QtCamViewfinderBufferListenerPrivate() {
  setSink(0);
  mutex.lock();
  qDeleteAll(handlers);
  handlers.clear();
  mutex.unlock();
}

void QtCamViewfinderBufferListenerPrivate::setSink(GstElement *sink) {
  if (QtCamViewfinderBufferListenerPrivate::sink == sink) {
    return;
  }

  if (QtCamViewfinderBufferListenerPrivate::sink) {
    removeBufferProbe();
  }

  QtCamViewfinderBufferListenerPrivate::sink = sink;

  if (QtCamViewfinderBufferListenerPrivate::sink) {
    addBufferProbe();
  }
}

void QtCamViewfinderBufferListenerPrivate::addHandler(QtCamViewfinderBufferHandler *handler) {
  QMutexLocker l(&mutex);
  if (handlers.indexOf(handler) == -1) {
    handlers << handler;
  }
}

void QtCamViewfinderBufferListenerPrivate::removeHandler(QtCamViewfinderBufferHandler *handler) {
  QMutexLocker l(&mutex);

  int index = handlers.indexOf(handler);
  if (index != -1) {
    handlers.takeAt(index);
  }
}

void QtCamViewfinderBufferListenerPrivate::addBufferProbe() {
  gst_object_ref(sink);

  pad = gst_element_get_static_pad(sink, "sink");
  if (!pad) {
    qCritical() << "addBufferProbe: failed to get sink pad";
    return;
  }

#if GST_CHECK_VERSION(1,0,0)
  probe_id = gst_pad_add_probe(pad, GST_PAD_PROBE_TYPE_BUFFER,
			       buffer_probe, this, NULL);
#else
  probe_id = gst_pad_add_buffer_probe(pad, G_CALLBACK(buffer_probe), this);
#endif

  // We keep the extra pad reference.
}

void QtCamViewfinderBufferListenerPrivate::removeBufferProbe() {
  if (pad && probe_id != 0) {
#if GST_CHECK_VERSION(1,0,0)
    gst_pad_remove_probe(pad, probe_id);
#else
    gst_pad_remove_buffer_probe(pad, probe_id);
#endif
  }

  if (pad) {
    gst_object_unref(pad);
    pad = 0;
  }

  gst_object_unref(sink);

  probe_id = 0;
}

#if GST_CHECK_VERSION(1,0,0)
GstPadProbeReturn QtCamViewfinderBufferListenerPrivate::buffer_probe(GstPad *pad,
								     GstPadProbeInfo *info,
								     gpointer user_data) {
  if (info->type != GST_PAD_PROBE_TYPE_BUFFER || !info->data) {
    return GST_PAD_PROBE_OK;
  }

  GstBuffer *buffer = (GstBuffer *) info->data;

#else
gboolean QtCamViewfinderBufferListenerPrivate::buffer_probe(GstPad *pad, GstBuffer *buffer,
							    gpointer user_data) {
#endif

  Q_UNUSED(pad);

  QtCamViewfinderBufferListenerPrivate *d = (QtCamViewfinderBufferListenerPrivate *) user_data;

  QMutexLocker l(&d->mutex);
  foreach (QtCamViewfinderBufferHandler *handler, d->handlers) {
    handler->handleBuffer(buffer);
  }

#if GST_CHECK_VERSION(1,0,0)
  return GST_PAD_PROBE_OK;
#else
  return TRUE;
#endif
}

QtCamViewfinderBufferListener::QtCamViewfinderBufferListener(QtCamDevicePrivate *d,
							     QObject *parent) :
  QObject(parent),
  d_ptr(new QtCamViewfinderBufferListenerPrivate(d)) {

}

QtCamViewfinderBufferListener::~QtCamViewfinderBufferListener() {
  delete d_ptr; d_ptr = 0;
}

void QtCamViewfinderBufferListener::addHandler(QtCamViewfinderBufferHandler *handler) {
  d_ptr->addHandler(handler);
}

void QtCamViewfinderBufferListener::removeHandler(QtCamViewfinderBufferHandler *handler) {
  d_ptr->removeHandler(handler);
}
