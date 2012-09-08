#include "qtcamaperture.h"
#include "qtcamcapability_p.h"

// TODO: hardcoded
#define APERTURE_MIN 0
#define APERTURE_MAX 255

QtCamAperture::QtCamAperture(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::Aperture, "aperture"),
		  parent) {
  // TODO: QML
}

QtCamAperture::~QtCamAperture() {

}

unsigned int QtCamAperture::value() {
  unsigned int val = 0;

  if (!d_ptr->uintValue(&val)) {
    return 0;
  }

  return val;
}

bool QtCamAperture::setValue(unsigned int val) {
  return d_ptr->setUintValue(val);
}

unsigned int QtCamAperture::minimumValue() {
  return APERTURE_MIN;
}

unsigned int QtCamAperture::maximumValue() {
  return APERTURE_MAX;
}
