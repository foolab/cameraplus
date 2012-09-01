#include "qtcamevcomp.h"
#include "qtcamcapability_p.h"

#define EV_COMP_MIN -2.5
#define EV_COMP_MAX +2.5

QtCamEvComp::QtCamEvComp(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::EvComp, "ev-compensation"),
		  parent) {

}

QtCamEvComp::~QtCamEvComp() {

}

qreal QtCamEvComp::value() {
  qreal val = 0.0;

  if (!d_ptr->floatValue(&val)) {
    return 0.0;
  }

  return val;
}

bool QtCamEvComp::setValue(qreal val) {
  return d_ptr->setFloatValue(val);
}

qreal QtCamEvComp::minimumValue() {
  return EV_COMP_MIN;
}

qreal QtCamEvComp::maximumValue() {
  return EV_COMP_MAX;
}
