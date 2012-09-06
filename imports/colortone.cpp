#include "colortone.h"
#include "camera.h"

ColorTone::ColorTone(QObject *parent) :
  Capability(parent),
  m_color(0) {

}

ColorTone::~ColorTone() {
  if (m_color) {
    delete m_color; m_color = 0;
  }
}

void ColorTone::deviceChanged() {
  if (m_color) {
    delete m_color; m_color = 0;
  }

  if (m_cam->device()) {
    m_color = new QtCamColorTone(m_cam->device(), this);
    QObject::connect(m_color, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
  }

  emit valueChanged();
}

ColorTone::ColorToneMode ColorTone::value() {
  return m_color ? (ColorToneMode)m_color->value() : ColorTone::Normal;
}

void ColorTone::setValue(const ColorTone::ColorToneMode& mode) {
  if (m_color) {
    m_color->setValue((QtCamColorTone::ColorToneMode)mode);
  }
}
