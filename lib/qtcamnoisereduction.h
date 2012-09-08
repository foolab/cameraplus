// -*- c++ -*-

#ifndef QT_CAM_NOISE_REDUCTION_H
#define QT_CAM_NOISE_REDUCTION_H

#include "qtcamcapability.h"

class QtCamNoiseReduction : public QtCamCapability {
  Q_OBJECT

public:
  typedef enum {
    None = 0x00000000,
    Bayer = 0x00000001,
    Ycc = 0x00000002,
    Temporal = 0x00000004,
    Fixed = 0x00000008,
    Extra = 0x00000010
  } NoiseReductionMode;

  QtCamNoiseReduction(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamNoiseReduction();

  NoiseReductionMode value();
  bool setValue(const NoiseReductionMode& mode);
};

#endif /* QT_CAM_NOISE_REDUCTION_H */
