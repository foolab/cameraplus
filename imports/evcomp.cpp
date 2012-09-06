#include "evcomp.h"
#include "camera.h"
#include "qtcamevcomp.h"

EvComp::EvComp(QObject *parent) :
  Capability(parent),
  m_evComp(0) {

}

EvComp::~EvComp() {
  if (m_evComp) {
    delete m_evComp; m_evComp = 0;
  }
}

void EvComp::deviceChanged() {
  if (m_evComp) {
    delete m_evComp; m_evComp = 0;
  }

  if (m_cam->device()) {
    m_evComp = new QtCamEvComp(m_cam->device(), this);
    QObject::connect(m_evComp, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
    QObject::connect(m_evComp, SIGNAL(minimumValueChanged()), this, SIGNAL(minimumChanged()));
    QObject::connect(m_evComp, SIGNAL(maximumValueChanged()), this, SIGNAL(maximunmChanged()));
  }

  emit valueChanged();
  emit minimumChanged();
  emit maximunmChanged();
}

qreal EvComp::value() {
  return m_evComp ? m_evComp->value() : 0.0;
}

void EvComp::setValue(qreal val) {
  if (m_evComp) {
    m_evComp->setValue(val);
  }
}

qreal EvComp::minimum() {
  return m_evComp ? m_evComp->minimumValue() : 0.0;
}

qreal EvComp::maximum() {
  return m_evComp ? m_evComp->maximumValue() : 0.0;
}
