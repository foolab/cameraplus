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

#ifndef VIEWFINDER_BUFFER_HANDLER_H
#define VIEWFINDER_BUFFER_HANDLER_H

#include <QObject>
#include <QMutex>
#include "qtcamviewfinderbufferhandler.h"
#include <QMetaMethod>

class Camera;
class QtCamDevice;

class ViewfinderBufferHandler : public QObject, public QtCamViewfinderBufferHandler {
  Q_OBJECT

  Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged);
  Q_PROPERTY(QObject* handler READ handler WRITE setHandler NOTIFY handlerChanged);

public:
  ViewfinderBufferHandler(QObject *parent = 0);
  ~ViewfinderBufferHandler();

  Camera *camera() const;
  void setCamera(Camera *camera);

  QObject *handler() const;
  void setHandler(QObject *handler);

  void handleBuffer(GstBuffer *buffer);

signals:
  void cameraChanged();
  void handlerChanged();

private slots:
  void deviceChanged();
  void deviceAboutToChange();

private:
  Camera *m_cam;
  QObject *m_handler;
  QtCamDevice *m_dev;
  QMutex m_mutex;
  QMetaMethod m_method;
};

#endif /* VIEWFINDER_BUFFER_HANDLER_H */
