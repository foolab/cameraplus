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

#ifndef QT_CAM_GST_MESSAGE_LISTENER_H
#define QT_CAM_GST_MESSAGE_LISTENER_H

#include <QObject>

class QtCamGstMessageListenerPrivate;
class QtCamGstMessageHandler;
class QtCamDevicePrivate;
typedef struct _GstBus GstBus;

class QtCamGstMessageListener : public QObject {
  Q_OBJECT

public:
  QtCamGstMessageListener(GstBus *bus, QtCamDevicePrivate *d, QObject *parent = 0);
  ~QtCamGstMessageListener();

  void addHandler(QtCamGstMessageHandler *handler);
  void removeHandler(QtCamGstMessageHandler *handler);
  void addSyncHandler(QtCamGstMessageHandler *handler);
  void removeSyncHandler(QtCamGstMessageHandler *handler);

  void flushMessages();

signals:
  void error(const QString& message, int code, const QString& debug);
  void starting();
  void started();
  void stopped();
  void stopping();

private:
  QtCamGstMessageListenerPrivate *d_ptr;
};

#endif /* QT_CAM_GST_MESSAGE_LISTENER_H */
