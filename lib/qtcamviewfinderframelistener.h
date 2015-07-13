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

#ifndef QT_CAM_VIEWFINDER_FRAME_LISTENER_H
#define QT_CAM_VIEWFINDER_FRAME_LISTENER_H

#include <QObject>

class QtCamViewfinderFrameListenerPrivate;
class QtCamViewfinderFrameHandler;
class QtCamViewfinderRenderer;
class QtCamDevicePrivate;

class QtCamViewfinderFrameListener : public QObject {
  Q_OBJECT

public:
  QtCamViewfinderFrameListener(QObject *parent = 0);
  ~QtCamViewfinderFrameListener();

  void addHandler(QtCamViewfinderFrameHandler *handler);
  void removeHandler(QtCamViewfinderFrameHandler *handler);

private:
  friend class QtCamDevice;

  QtCamViewfinderFrameListenerPrivate *d_ptr;
};

#endif /* QT_CAM_VIEWFINDER_FRAME_LISTENER_H */
