// -*- c++ -*-

#ifndef QT_CAM_ISO_H
#define QT_CAM_ISO_H

#include "qtcamcapability.h"

class QtCamIso : public QtCamCapability {
  Q_OBJECT

public:
  QtCamIso(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamIso();

  unsigned int value();
  bool setValue(unsigned int val);

  unsigned int minimumValue();
  unsigned int maximumValue();
};

#endif /* QT_CAM_ISO_H */
