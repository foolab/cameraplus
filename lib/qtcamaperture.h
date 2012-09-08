// -*- c++ -*-

#ifndef QT_CAM_APERTURE_H
#define QT_CAM_APERTURE_H

#include "qtcamcapability.h"

class QtCamAperture : public QtCamCapability {
  Q_OBJECT

public:
  QtCamAperture(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamAperture();

  unsigned int value();
  bool setValue(unsigned int val);

  unsigned int minimumValue();
  unsigned int maximumValue();
};

#endif /* QT_CAM_APERTURE_H */
