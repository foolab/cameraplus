// -*- c++ -*-

#ifndef QT_CAM_EXPOSURE_H
#define QT_CAM_EXPOSURE_H

#include "qtcamcapability.h"

class QtCamExposure : public QtCamCapability {
  Q_OBJECT

public:
  QtCamExposure(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamExposure();

  unsigned int value();
  bool setValue(unsigned int val);

  unsigned int minimumValue();
  unsigned int maximumValue();
};

#endif /* QT_CAM_EXPOSURE_H */
