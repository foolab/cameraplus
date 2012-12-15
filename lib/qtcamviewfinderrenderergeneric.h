// -*- c++ -*-

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

#ifndef QT_CAM_VIEWFINDER_RENDERER_GENERIC_H
#define QT_CAM_VIEWFINDER_RENDERER_GENERIC_H

#include "qtcamviewfinderrenderer.h"
#include <QImage>
#include <QMutex>

class QtCamViewfinderRendererGeneric : public QtCamViewfinderRenderer {
  Q_OBJECT

public:
  Q_INVOKABLE QtCamViewfinderRendererGeneric(QtCamConfig *config, QObject *parent = 0);

  ~QtCamViewfinderRendererGeneric();

  virtual void paint(QPainter *painter);
  virtual void resize(const QSizeF& size);
  virtual void reset();
  virtual GstElement *sinkElement();
  QRectF renderArea();

private:
  static void on_gst_buffer(GstElement *element, GstBuffer *buf, GstPad *pad,
			    QtCamViewfinderRendererGeneric *q);

  static void sink_notify(QtCamViewfinderRendererGeneric *q, GObject *object,
			  gboolean is_last_ref);

  GstElement *m_elem;
  GstElement *m_sink;
  QImage m_image;
  QMutex m_mutex;
  QSizeF m_size;
  unsigned long m_id;
};

#endif /* QT_CAM_VIEWFINDER_RENDERER_GENERIC_H */
