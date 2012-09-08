#include "qtcamexposure.h"
#include "qtcamcapability_p.h"

// TODO: hardcoded
#define EXPOSURE_MIN 0
#define EXPOSURE_MAX 4294967295u

QtCamExposure::QtCamExposure(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::Exposure, "exposure"),
		  parent) {
  // TODO: QML
}

QtCamExposure::~QtCamExposure() {

}

unsigned int QtCamExposure::value() {
  unsigned int val = 0;

  if (!d_ptr->uintValue(&val)) {
    return 0;
  }

  return val;
}

bool QtCamExposure::setValue(unsigned int val) {
  return d_ptr->setUintValue(val);
}

unsigned int QtCamExposure::minimumValue() {
  return EXPOSURE_MIN;
}

unsigned int QtCamExposure::maximumValue() {
  return EXPOSURE_MAX;
}
