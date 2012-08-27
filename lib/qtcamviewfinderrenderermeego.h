// -*- c++ -*-

#ifndef QT_CAM_VIEWFINDER_RENDERER_MEEGO_H
#define QT_CAM_VIEWFINDER_RENDERER_MEEGO_H

#include "qtcamviewfinderrenderer.h"
#include <QImage>
#include <QMutex>
#include <QMatrix4x4>
#include <GLES2/gl2.h>

class QGLShaderProgram;

class QtCamViewfinderRendererMeeGo : public QtCamViewfinderRenderer {
  Q_OBJECT

public:
  Q_INVOKABLE QtCamViewfinderRendererMeeGo(QtCamConfig *config, QObject *parent = 0);

  ~QtCamViewfinderRendererMeeGo();

  virtual void paint(QPainter *painter);
  virtual void resize(const QSizeF& size);
  virtual void reset();
  virtual GstElement *sinkElement();

private slots:
  void setVideoSize(const QSizeF& size);

private:
  static void frame_ready(GstElement *sink, int frame, QtCamViewfinderRendererMeeGo *r);
  static void sink_notify(QtCamViewfinderRendererMeeGo *q, GObject *object, gboolean is_last_ref);
  static void sink_caps_changed(GObject *obj, GParamSpec *pspec, QtCamViewfinderRendererMeeGo *q);

  void calculateProjectionMatrix(const QRectF& rect);
  void createProgram();
  void paintFrame(QPainter *painter, int frame);
  void calculateCoords();
  QRectF renderArea();

  QtCamConfig *m_conf;
  GstElement *m_sink;
  QMutex m_frameMutex;
  int m_frame;
  unsigned long m_id;
  unsigned long m_notify;
  bool m_needsInit;
  QGLShaderProgram *m_program;
  QMatrix4x4 m_projectionMatrix;
  GLfloat m_vertexCoords[8];
  GLfloat m_texCoords[8];
  QSizeF m_size;
  QSizeF m_videoSize;
  QRectF m_renderArea;
};

#endif /* QT_CAM_VIEWFINDER_RENDERER_MEEGO_H */
