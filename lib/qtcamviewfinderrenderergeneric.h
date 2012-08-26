// -*- c++ -*-

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
};

#endif /* QT_CAM_VIEWFINDER_RENDERER_GENERIC_H */
