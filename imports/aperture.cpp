#include "aperture.h"
#include "camera.h"
#include "qtcamaperture.h"

Aperture::Aperture(QObject *parent) :
  Capability(parent),
  m_aperture(0) {

}

Aperture::~Aperture() {
  if (m_aperture) {
    delete m_aperture; m_aperture = 0;
  }
}

void Aperture::deviceChanged() {
  if (m_aperture) {
    delete m_aperture; m_aperture = 0;
  }

  if (m_cam->device()) {
    m_aperture = new QtCamAperture(m_cam->device(), this);
    QObject::connect(m_aperture, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
    QObject::connect(m_aperture, SIGNAL(minimumValueChanged()), this, SIGNAL(minimumChanged()));
    QObject::connect(m_aperture, SIGNAL(maximumValueChanged()), this, SIGNAL(maximunmChanged()));
  }

  emit valueChanged();
  emit minimumChanged();
  emit maximunmChanged();
}

unsigned int Aperture::value() {
  return m_aperture ? m_aperture->value() : 0;
}

void Aperture::setValue(unsigned int val) {
  if (m_aperture) {
    m_aperture->setValue(val);
  }
}

unsigned int Aperture::minimum() {
  return m_aperture ? m_aperture->minimumValue() : 0;
}

unsigned int Aperture::maximum() {
  return m_aperture ? m_aperture->maximumValue() : 0;
}
