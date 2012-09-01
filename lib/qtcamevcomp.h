// -*- c++ -*-

#ifndef QT_CAM_EV_COMP_H
#define QT_CAM_EV_COMP_H

#include "qtcamcapability.h"

class QtCamEvComp : public QtCamCapability {
  Q_OBJECT

public:
  QtCamEvComp(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamEvComp();

  qreal value();
  bool setValue(qreal val);

  qreal minimumValue();
  qreal maximumValue();
};

#endif /* QT_CAM_EV_COMP_H */
