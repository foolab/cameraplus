#include "qtcamnoisereduction.h"
#include "qtcamcapability_p.h"

QtCamNoiseReduction::QtCamNoiseReduction(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::NoiseReduction,
					     "noise-reduction"), parent) {

}

QtCamNoiseReduction::~QtCamNoiseReduction() {

}

QtCamNoiseReduction::NoiseReductionMode QtCamNoiseReduction::value() {
  int val = 0;
  if (!d_ptr->intValue(&val)) {
    return QtCamNoiseReduction::None;
  }

  switch (val) {
  case QtCamNoiseReduction::Bayer:
  case QtCamNoiseReduction::Ycc:
  case QtCamNoiseReduction::Temporal:
  case QtCamNoiseReduction::Fixed:
  case QtCamNoiseReduction::Extra:
    return (QtCamNoiseReduction::NoiseReductionMode)val;

  default:
    return QtCamNoiseReduction::None;
  }
}

bool QtCamNoiseReduction::setValue(const QtCamNoiseReduction::NoiseReductionMode& mode) {
  return d_ptr->setIntValue(mode, false);
}
