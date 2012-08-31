// -*- c++ -*-

#ifndef QT_CAM_FLASH_H
#define QT_CAM_FLASH_H

#include "qtcamcapability.h"

class QtCamFlash : public QtCamCapability {
  Q_OBJECT

public:
  typedef enum {
    Auto = 0,
    Off = 1,
    On = 2,
    FillIn = 3,
    RedEye = 4
  } FlashMode;

  QtCamFlash(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamFlash();

  FlashMode value();
  bool setValue(const FlashMode& mode);
};

#endif /* QT_CAM_FLASH_H */
