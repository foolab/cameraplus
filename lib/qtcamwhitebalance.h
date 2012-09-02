// -*- c++ -*-

#ifndef QT_CAM_WHITE_BALANCE_H
#define QT_CAM_WHITE_BALANCE_H

#include "qtcamcapability.h"

class QtCamWhiteBalance : public QtCamCapability {
  Q_OBJECT

public:
  typedef enum {
    Auto = 0,
    Daylight = 1,
    Cloudy = 2,
    Sunset = 3,
    Tungsten = 4,
    Flourescent = 5
  } WhiteBalanceMode;

  QtCamWhiteBalance(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamWhiteBalance();

  WhiteBalanceMode value();
  bool setValue(const WhiteBalanceMode& mode);
};

#endif /* QT_CAM_WHITE_BALANCE_H */
