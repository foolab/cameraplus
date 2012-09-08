#include "qtcamflickerreduction.h"
#include "qtcamcapability_p.h"

QtCamFlickerReduction::QtCamFlickerReduction(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::FlickerReduction,
					     "flicker-mode"), parent) {

}

QtCamFlickerReduction::~QtCamFlickerReduction() {

}

QtCamFlickerReduction::FlickerReductionMode QtCamFlickerReduction::value() {
  int val = 0;
  if (!d_ptr->intValue(&val)) {
    return QtCamFlickerReduction::Auto;
  }

  switch (val) {
  case QtCamFlickerReduction::Off:
  case QtCamFlickerReduction::FiftyHz:
  case QtCamFlickerReduction::SixtyHz:
    return (QtCamFlickerReduction::FlickerReductionMode)val;

  default:
    return QtCamFlickerReduction::Auto;
  }
}

bool QtCamFlickerReduction::setValue(const QtCamFlickerReduction::FlickerReductionMode& mode) {
  return d_ptr->setIntValue(mode, false);
}
