#include "qtcamiso.h"
#include "qtcamcapability_p.h"

// TODO: hardcoded
#define ISO_MIN 0
#define ISO_MAX 6400

QtCamIso::QtCamIso(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::IsoSpeed, "iso-speed"),
		  parent) {

}

QtCamIso::~QtCamIso() {

}

unsigned int QtCamIso::value() {
  unsigned int val = 0;

  if (!d_ptr->uintValue(&val)) {
    return 0;
  }

  return val;
}

bool QtCamIso::setValue(unsigned int val) {
  return d_ptr->setUintValue(val);
}

unsigned int QtCamIso::minimumValue() {
  return ISO_MIN;
}

unsigned int QtCamIso::maximumValue() {
  return ISO_MAX;
}
