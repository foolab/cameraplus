#include "noisereduction.h"
#include "camera.h"

NoiseReduction::NoiseReduction(QObject *parent) :
  Capability(parent),
  m_nr(0) {

}

NoiseReduction::~NoiseReduction() {
  if (m_nr) {
    delete m_nr; m_nr = 0;
  }
}

void NoiseReduction::deviceChanged() {
  if (m_nr) {
    delete m_nr; m_nr = 0;
  }

  if (m_cam->device()) {
    m_nr = new QtCamNoiseReduction(m_cam->device(), this);
    QObject::connect(m_nr, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
  }

  emit valueChanged();
}

NoiseReduction::NoiseReductionMode NoiseReduction::value() {
  return m_nr ? (NoiseReductionMode)m_nr->value() : NoiseReduction::None;
}

void NoiseReduction::setValue(const NoiseReduction::NoiseReductionMode& mode) {
  if (m_nr) {
    m_nr->setValue((QtCamNoiseReduction::NoiseReductionMode)mode);
  }
}
