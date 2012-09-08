// -*- c++ -*-

#ifndef QT_CAM_FLICKER_REDUCTION_H
#define QT_CAM_FLICKER_REDUCTION_H

#include "qtcamcapability.h"

class QtCamFlickerReduction : public QtCamCapability {
  Q_OBJECT

public:
  typedef enum {
    Off = 0,
    FiftyHz = 1,
    SixtyHz = 2,
    Auto = 3
  } FlickerReductionMode;

  QtCamFlickerReduction(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamFlickerReduction();

  FlickerReductionMode value();
  bool setValue(const FlickerReductionMode& mode);
};

#endif /* QT_CAM_FLICKER_REDUCTION_H */
