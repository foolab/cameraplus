#include "qtcamwb.h"
#include "qtcamcapability_p.h"

QtCamWb::QtCamWb(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::Wb, "white-balance-mode"),
		  parent) {

}

QtCamWb::~QtCamWb() {

}

QtCamWb::WbMode QtCamWb::value() {
  int val = 0;
  if (!d_ptr->intValue(&val)) {
    return QtCamWb::Auto;
  }

  switch (val) {
  case QtCamWb::Daylight:
  case QtCamWb::Cloudy:
  case QtCamWb::Sunset:
  case QtCamWb::Tungsten:
  case QtCamWb::Flourescent:
    return (QtCamWb::WbMode)val;

  default:
    return QtCamWb::Auto;
  }
}

bool QtCamWb::setValue(const QtCamWb::WbMode& mode) {
  return d_ptr->setIntValue(mode);
}
