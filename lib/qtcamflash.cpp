#include "qtcamflash.h"
#include "qtcamcapability_p.h"

QtCamFlash::QtCamFlash(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::Flash, "flash-mode"), parent) {

}

QtCamFlash::~QtCamFlash() {

}

QtCamFlash::FlashMode QtCamFlash::value() {
  int val = 0;
  if (!d_ptr->intValue(&val)) {
    return QtCamFlash::Auto;
  }

  switch (val) {
  case QtCamFlash::On:
  case QtCamFlash::Off:
  case QtCamFlash::FillIn:
  case QtCamFlash::RedEye:
    return (QtCamFlash::FlashMode)val;

  default:
    return QtCamFlash::Auto;
  }
}

bool QtCamFlash::setValue(const QtCamFlash::FlashMode& mode) {
  return d_ptr->setIntValue(mode);
}
