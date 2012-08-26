// -*- c++ -*-

#ifndef QT_CAM_VIEWFINDER_RENDERER_H
#define QT_CAM_VIEWFINDER_RENDERER_H

#include <QObject>
#include <gst/gst.h>

class QtCamConfig;
class QMetaObject;
class QPainter;
class QSizeF;

class QtCamViewfinderRenderer : public QObject {
  Q_OBJECT

public:
  static QtCamViewfinderRenderer *create(QtCamConfig *config, QObject *parent = 0);
  static int registerRenderer(const QString& key, const QMetaObject& meta);

  virtual ~QtCamViewfinderRenderer();

  virtual void paint(QPainter *painter) = 0;
  virtual void resize(const QSizeF& size) = 0;
  virtual void reset() = 0;
  virtual GstElement *sinkElement() = 0;

protected:
  QtCamViewfinderRenderer(QtCamConfig *config, QObject *parent = 0);

signals:
  void updateRequested();
};

#define QT_CAM_VIEWFINDER_RENDERER(key, klass) \
static int klass_seq = QtCamViewfinderRenderer::registerRenderer(key, klass::staticMetaObject)

#endif /* QT_CAM_VIEWFINDER_RENDERER_H */
