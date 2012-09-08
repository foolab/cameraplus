#include "flickerreduction.h"
#include "camera.h"

FlickerReduction::FlickerReduction(QObject *parent) :
  Capability(parent),
  m_fr(0) {

}

FlickerReduction::~FlickerReduction() {
  if (m_fr) {
    delete m_fr; m_fr = 0;
  }
}

void FlickerReduction::deviceChanged() {
  if (m_fr) {
    delete m_fr; m_fr = 0;
  }

  if (m_cam->device()) {
    m_fr = new QtCamFlickerReduction(m_cam->device(), this);
    QObject::connect(m_fr, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
  }

  emit valueChanged();
}

FlickerReduction::FlickerReductionMode FlickerReduction::value() {
  return m_fr ? (FlickerReductionMode)m_fr->value() : FlickerReduction::Off;
}

void FlickerReduction::setValue(const FlickerReduction::FlickerReductionMode& mode) {
  if (m_fr) {
    m_fr->setValue((QtCamFlickerReduction::FlickerReductionMode)mode);
  }
}
