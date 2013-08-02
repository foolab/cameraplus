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

#ifndef QT_CAM_VIEWFINDER_H
#define QT_CAM_VIEWFINDER_H

#include <gst/gst.h>
#include <QRectF>

class QtCamDevice;

class QtCamViewfinder {
public:
  QtCamViewfinder();
  virtual ~QtCamViewfinder();

  virtual GstElement *sinkElement() = 0;
  virtual void stop() = 0;

  virtual QRectF renderArea() const = 0;
  virtual QSizeF videoResolution() const = 0;
};

#endif /* QT_CAM_VIEWFINDER_H */
