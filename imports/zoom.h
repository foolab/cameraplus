// -*- c++ -*-

#ifndef ZOOM_H
#define ZOOM_H

#include "capability.h"

class QtCamZoom;

class Zoom : public Capability {
  Q_OBJECT

  Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged);
  Q_PROPERTY(qreal minimum READ minimum NOTIFY minimumChanged);
  Q_PROPERTY(qreal maximum READ maximum NOTIFY maximunmChanged);

public:
  Zoom(QObject *parent = 0);
  ~Zoom();

  qreal value();
  void setValue(qreal val);

  qreal minimum();
  qreal maximum();

signals:
  void valueChanged();
  void minimumChanged();
  void maximunmChanged();

private:
  virtual void deviceChanged();

  QtCamZoom *m_zoom;
};

#endif /* ZOOM_H */
