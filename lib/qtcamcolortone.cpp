#include "qtcamcolortone.h"
#include "qtcamcapability_p.h"

QtCamColorTone::QtCamColorTone(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::ColourTone, "colour-tone-mode"),
		  parent) {

}

QtCamColorTone::~QtCamColorTone() {

}

QtCamColorTone::ColorToneMode QtCamColorTone::value() {
  int val = 0;
  if (!d_ptr->intValue(&val)) {
    return QtCamColorTone::Normal;
  }

  switch (val) {
  case QtCamColorTone::Sepia:
  case QtCamColorTone::Negative:
  case QtCamColorTone::GrayScale:
  case QtCamColorTone::Natural:
  case QtCamColorTone::Vivid:
  case QtCamColorTone::ColorSwap:
  case QtCamColorTone::Solarize:
  case QtCamColorTone::OutOfFocus:
  case QtCamColorTone::SkyBlue:
  case QtCamColorTone::GrassGreen:
  case QtCamColorTone::SkinWhite:
    return (QtCamColorTone::ColorToneMode)val;

  default:
    return QtCamColorTone::Normal;
  }
}

bool QtCamColorTone::setValue(const QtCamColorTone::ColorToneMode& mode) {
  return d_ptr->setIntValue(mode, false);
}
