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

#include "qtcamgstreamermessagelistener.h"
#include "qtcamgstreamermessagehandler.h"
#include <QMultiMap>
#include <QMutex>
#include <QDebug>
#include "qtcamdevice_p.h"

class QtCamGStreamerMessageListenerPrivate {
public:
  QMultiMap<QString, QtCamGStreamerMessageHandler *> handlers;
  QMultiMap<QString, QtCamGStreamerMessageHandler *> syncHandlers;

  int handleMessage(GstMessage *message, QMultiMap<QString, QtCamGStreamerMessageHandler *>& map) {
    const GstStructure *s = gst_message_get_structure(message);
    if (!s) {
      return 0;
    }

#if 0
    qDebug() << "Message" << gst_structure_get_name(s);
#endif

    QList<QtCamGStreamerMessageHandler *> list = map.values(gst_structure_get_name(s));

    foreach (QtCamGStreamerMessageHandler *handler, list) {
      handler->handleMessage(message);
    }

    return list.size();
  }

  void handleMessage(GstMessage *message) {

    switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_ELEMENT:
      handleMessage(message, handlers);
      break;

    case GST_MESSAGE_ERROR: {
      GError *err = NULL;
      gchar *debug;

      gst_message_parse_error(message, &err, &debug);

      QMetaObject::invokeMethod(q_ptr, "error", Q_ARG(QString, err->message),
				Q_ARG(int, err->code), Q_ARG(QString, debug));

      qDebug() << "Error" << err->message << ":" << debug;

      g_error_free(err);
      g_free(debug);
    }
      break;

    case GST_MESSAGE_WARNING: {
      GError *err = NULL;
      gchar *debug;

      gst_message_parse_warning(message, &err, &debug);

      qDebug() << "Warning" << err->message << ":" << debug;

      g_error_free(err);
      g_free(debug);
    }
      break;

    case GST_MESSAGE_INFO: {
      GError *err = NULL;
      gchar *debug;

      gst_message_parse_info(message, &err, &debug);

      qDebug() << "Info" << err->message << ":" << debug;

      g_error_free(err);
      g_free(debug);
    }
      break;

    case GST_MESSAGE_STATE_CHANGED: {
      if (GST_ELEMENT(GST_MESSAGE_SRC(message)) != dev->cameraBin) {
	break;
      }

      GstState oldState, newState, pending;
      gst_message_parse_state_changed(message, &oldState, &newState, &pending);
      if (oldState == GST_STATE_PAUSED && newState == GST_STATE_PLAYING) {
	QMetaObject::invokeMethod(q_ptr, "started");
      }
      else if (oldState == GST_STATE_PLAYING && newState == GST_STATE_PAUSED) {
	QMetaObject::invokeMethod(q_ptr, "stopping");
      }
      else if (oldState == GST_STATE_READY && newState == GST_STATE_NULL) {
	QMetaObject::invokeMethod(q_ptr, "stopped");
      }
    }
      break;

    default:
      break;
    }
  }

  bool handleSyncMessage(GstMessage *message) {
    QMutexLocker locker(&syncMutex);

    if (handleMessage(message, syncHandlers) != 0) {
      return true;
    }

    return false;
  }

  void addHandler(QtCamGStreamerMessageHandler *handler,
		  QMultiMap<QString, QtCamGStreamerMessageHandler *>& map) {
    if (!map.contains(handler->messageName(), handler)) {
      map.insert(handler->messageName(), handler);
      handler->setParent(q_ptr);
    }
  }

  void removeHandler(QtCamGStreamerMessageHandler *handler,
		     QMultiMap<QString, QtCamGStreamerMessageHandler *>& map) {
    map.remove(handler->messageName(), handler);
    handler->setParent(0);
  }

  QMutex syncMutex;

  GstBus *bus;

  QtCamDevicePrivate *dev;

  guint watchId;

  QtCamGStreamerMessageListener *q_ptr;
};

gboolean async_handler(GstBus *bus, GstMessage *message, gpointer data)
{
  Q_UNUSED(bus);

  QtCamGStreamerMessageListenerPrivate *d_ptr =
    static_cast<QtCamGStreamerMessageListenerPrivate *>(data);

  d_ptr->handleMessage(message);

  // Call us again
  return TRUE;
}

GstBusSyncReply sync_handler(GstBus *bus, GstMessage *message, gpointer data) {
  Q_UNUSED(bus);

  QtCamGStreamerMessageListenerPrivate *d_ptr =
    static_cast<QtCamGStreamerMessageListenerPrivate *>(data);

  if (d_ptr->handleSyncMessage(message)) {
    gst_message_unref(message);
    return GST_BUS_DROP;
  }

  return GST_BUS_PASS;
}

QtCamGStreamerMessageListener::QtCamGStreamerMessageListener(GstBus *bus,
							     QtCamDevicePrivate *d,
							     QObject *parent) :
  QObject(parent), d_ptr(new QtCamGStreamerMessageListenerPrivate) {

  d_ptr->dev = d;
  d_ptr->bus = bus;
  d_ptr->q_ptr = this;

  d_ptr->watchId = gst_bus_add_watch(d_ptr->bus, async_handler, d_ptr);

  gst_bus_set_sync_handler(d_ptr->bus, sync_handler, d_ptr);
}

QtCamGStreamerMessageListener::~QtCamGStreamerMessageListener() {
  g_source_remove(d_ptr->watchId);
  gst_bus_set_sync_handler(d_ptr->bus, NULL, NULL);

  qDeleteAll(d_ptr->handlers);

  d_ptr->syncMutex.lock();
  qDeleteAll(d_ptr->syncHandlers);
  d_ptr->syncMutex.unlock();

  gst_object_unref(d_ptr->bus);

  delete d_ptr; d_ptr = 0;
}

void QtCamGStreamerMessageListener::addHandler(QtCamGStreamerMessageHandler *handler) {
  d_ptr->addHandler(handler, d_ptr->handlers);
}

void QtCamGStreamerMessageListener::removeHandler(QtCamGStreamerMessageHandler *handler) {
  d_ptr->removeHandler(handler, d_ptr->handlers);
}

void QtCamGStreamerMessageListener::addSyncHandler(QtCamGStreamerMessageHandler *handler) {
  QMutexLocker locker(&d_ptr->syncMutex);

  d_ptr->addHandler(handler, d_ptr->syncHandlers);
}

void QtCamGStreamerMessageListener::removeSyncHandler(QtCamGStreamerMessageHandler *handler) {
  QMutexLocker locker(&d_ptr->syncMutex);

  d_ptr->removeHandler(handler, d_ptr->syncHandlers);
}

void QtCamGStreamerMessageListener::flushMessages() {
  GstMessage *message = 0;

  while ((message = gst_bus_pop(d_ptr->bus))) {
    d_ptr->handleMessage(message);
    gst_message_unref(message);
  }
}
