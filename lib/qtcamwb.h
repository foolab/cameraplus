// -*- c++ -*-

#ifndef QT_CAM_WB_H
#define QT_CAM_WB_H

#include "qtcamcapability.h"

class QtCamWb : public QtCamCapability {
  Q_OBJECT

public:
  typedef enum {
    Auto = 0,
    Daylight = 1,
    Cloudy = 2,
    Sunset = 3,
    Tungsten = 4,
    Flourescent = 5
  } WbMode;

  QtCamWb(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamWb();

  WbMode value();
  bool setValue(const WbMode& mode);
};

#endif /* QT_CAM_WB_H */
