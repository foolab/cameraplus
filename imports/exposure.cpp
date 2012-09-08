#include "exposure.h"
#include "camera.h"
#include "qtcamexposure.h"

Exposure::Exposure(QObject *parent) :
  Capability(parent),
  m_exposure(0) {

}

Exposure::~Exposure() {
  if (m_exposure) {
    delete m_exposure; m_exposure = 0;
  }
}

void Exposure::deviceChanged() {
  if (m_exposure) {
    delete m_exposure; m_exposure = 0;
  }

  if (m_cam->device()) {
    m_exposure = new QtCamExposure(m_cam->device(), this);
    QObject::connect(m_exposure, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
    QObject::connect(m_exposure, SIGNAL(minimumValueChanged()), this, SIGNAL(minimumChanged()));
    QObject::connect(m_exposure, SIGNAL(maximumValueChanged()), this, SIGNAL(maximunmChanged()));
  }

  emit valueChanged();
  emit minimumChanged();
  emit maximunmChanged();
}

unsigned int Exposure::value() {
  return m_exposure ? m_exposure->value() : 0;
}

void Exposure::setValue(unsigned int val) {
  if (m_exposure) {
    m_exposure->setValue(val);
  }
}

unsigned int Exposure::minimum() {
  return m_exposure ? m_exposure->minimumValue() : 0;
}

unsigned int Exposure::maximum() {
  return m_exposure ? m_exposure->maximumValue() : 0;
}
