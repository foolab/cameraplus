// -*- c++ -*-

#ifndef QT_CAM_GRAPHICS_VIEWFINDER_H
#define QT_CAM_GRAPHICS_VIEWFINDER_H

#include "qtcamviewfinder.h"
#include <QGraphicsWidget>

class QtCamGraphicsViewfinderPrivate;
class QtCamConfig;

class QtCamGraphicsViewfinder : public QGraphicsWidget, public QtCamViewfinder {
  Q_OBJECT

public:
  QtCamGraphicsViewfinder(QtCamConfig *config, QGraphicsItem *parent = 0);
  virtual ~QtCamGraphicsViewfinder();

  virtual GstElement *sinkElement();
  virtual bool setDevice(QtCamDevice *device);
  virtual void stop();

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		     QWidget *widget = 0);

protected:
  void resizeEvent(QGraphicsSceneResizeEvent *event);

private slots:
  void updateRequested();

private:
  QtCamGraphicsViewfinderPrivate *d_ptr;
};

#endif /* QT_CAM_GRAPHICS_VIEWFINDER_H */
