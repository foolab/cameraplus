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

#ifndef QT_CAM_VIDEO_MODE_P_H
#define QT_CAM_VIDEO_MODE_P_H

#include <QObject>
#include "qtcammode_p.h"

class StreamRewriter;

class QtCamVideoModePrivate : public QObject, public QtCamModePrivate {
  Q_OBJECT

public:
  QtCamVideoModePrivate(QtCamDevicePrivate *dev);
  ~QtCamVideoModePrivate();

  StreamRewriter *createRewriter(const char *prop, const char *name, bool copy);
  void createRewriters();
  void clearRewriters();

  QtCamResolution resolution;
  StreamRewriter *audio;
  StreamRewriter *video;

public slots:
  void _d_idleStateChanged(bool isIdle);
};

#endif /* QT_CAM_VIDEO_MODE_P_H */
