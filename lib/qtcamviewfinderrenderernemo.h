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

#ifndef QT_CAM_VIEWFINDER_RENDERER_MEEGO_H
#define QT_CAM_VIEWFINDER_RENDERER_MEEGO_H

#include "qtcamviewfinderrenderer.h"
#include <QMutex>
#include <QMatrix4x4>
#include <gst/video/video.h>
#include <QOpenGLShaderProgram>
#include <gst/meta/nemometa.h>

typedef void *EGLDisplay;

class QtCamViewfinderRendererNemo : public QtCamViewfinderRenderer {
  Q_OBJECT

public:
  Q_INVOKABLE QtCamViewfinderRendererNemo(QtCamConfig *config, QObject *parent = 0);

  ~QtCamViewfinderRendererNemo();

  virtual void paint(const QMatrix4x4& matrix, const QRectF& viewport);
  virtual void resize(const QSizeF& size);
  virtual void reset();
  virtual void start();
  virtual GstElement *sinkElement();

  QRectF renderArea();
  QSizeF videoResolution();

  bool needsNativePainting();

  virtual void setApplicationOrientationAngle(int angle);

private slots:
  void setVideoSize(const QSizeF& size);

private:
  static void frame_ready(GstElement *sink, int frame, QtCamViewfinderRendererNemo *r);
  static void sink_notify(QtCamViewfinderRendererNemo *q, GObject *object, gboolean is_last_ref);
  static void sink_caps_changed(GObject *obj, GParamSpec *pspec, QtCamViewfinderRendererNemo *q);

  void calculateProjectionMatrix(const QRectF& rect);
  void createProgram();
  void paintFrame(const QMatrix4x4& matrix, int frame);
  void calculateVertexCoords();

  void cleanup();
  void updateCropInfo(const GstVideoCropMeta *crop, GLfloat *texCoords);
  void updateTextureCoordinates(NemoGstBufferOrientationMeta *meta, GLfloat *texCoords);

  QtCamConfig *m_conf;
  GstElement *m_sink;
  QMutex m_frameMutex;
  int m_frame;
  unsigned long m_id;
  unsigned long m_notify;
  bool m_needsInit;
  QOpenGLShaderProgram *m_program;
  QMatrix4x4 m_projectionMatrix;
  GLfloat m_vertexCoords[8];
  QSizeF m_size;
  QSizeF m_videoSize;
  QRectF m_renderArea;
  EGLDisplay m_dpy;
  bool m_displaySet;
  bool m_started;
  int m_angle;
};

#endif /* QT_CAM_VIEWFINDER_RENDERER_MEEGO_H */
