// -*- c++ -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
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

#ifndef QT_CAM_GSTREAMER_MESSAGE_LISTENER_H
#define QT_CAM_GSTREAMER_MESSAGE_LISTENER_H

#include <QObject>
#include <gst/gst.h>

class QtCamGStreamerMessageListenerPrivate;
class QtCamGStreamerMessageHandler;
class QtCamDevicePrivate;

class QtCamGStreamerMessageListener : public QObject {
  Q_OBJECT

public:
  QtCamGStreamerMessageListener(GstBus *bus, QtCamDevicePrivate *d, QObject *parent = 0);
  ~QtCamGStreamerMessageListener();

  void addHandler(QtCamGStreamerMessageHandler *handler);
  void removeHandler(QtCamGStreamerMessageHandler *handler);
  void addSyncHandler(QtCamGStreamerMessageHandler *handler);
  void removeSyncHandler(QtCamGStreamerMessageHandler *handler);

  void flushMessages();

signals:
  void error(const QString& message, int code, const QString& debug);
  void starting();
  void started();
  void stopped();
  void stopping();

private:
  QtCamGStreamerMessageListenerPrivate *d_ptr;
};

#endif /* QT_CAM_GSTREAMER_MESSAGE_LISTENER_H */
