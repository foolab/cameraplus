#include "qtcamwhitebalance.h"
#include "qtcamcapability_p.h"

QtCamWhiteBalance::QtCamWhiteBalance(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::WhiteBalance,
					     "white-balance-mode"),
		  parent) {

}

QtCamWhiteBalance::~QtCamWhiteBalance() {

}

QtCamWhiteBalance::WhiteBalanceMode QtCamWhiteBalance::value() {
  int val = 0;
  if (!d_ptr->intValue(&val)) {
    return QtCamWhiteBalance::Auto;
  }

  switch (val) {
  case QtCamWhiteBalance::Daylight:
  case QtCamWhiteBalance::Cloudy:
  case QtCamWhiteBalance::Sunset:
  case QtCamWhiteBalance::Tungsten:
  case QtCamWhiteBalance::Flourescent:
    return (QtCamWhiteBalance::WhiteBalanceMode)val;

  default:
    return QtCamWhiteBalance::Auto;
  }
}

bool QtCamWhiteBalance::setValue(const QtCamWhiteBalance::WhiteBalanceMode& mode) {
  return d_ptr->setIntValue(mode);
}
