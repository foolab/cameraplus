#include "iso.h"
#include "camera.h"
#include "qtcamiso.h"

Iso::Iso(QObject *parent) :
  Capability(parent),
  m_iso(0) {

}

Iso::~Iso() {
  if (m_iso) {
    delete m_iso; m_iso = 0;
  }
}

void Iso::deviceChanged() {
  if (m_iso) {
    delete m_iso; m_iso = 0;
  }

  if (m_cam->device()) {
    m_iso = new QtCamIso(m_cam->device(), this);
    QObject::connect(m_iso, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
    QObject::connect(m_iso, SIGNAL(minimumValueChanged()), this, SIGNAL(minimumChanged()));
    QObject::connect(m_iso, SIGNAL(maximumValueChanged()), this, SIGNAL(maximunmChanged()));
  }

  emit valueChanged();
  emit minimumChanged();
  emit maximunmChanged();
}

unsigned int Iso::value() {
  return m_iso ? m_iso->value() : 0;
}

void Iso::setValue(unsigned int val) {
  if (m_iso) {
    m_iso->setValue(val);
  }
}

unsigned int Iso::minimum() {
  return m_iso ? m_iso->minimumValue() : 0;
}

unsigned int Iso::maximum() {
  return m_iso ? m_iso->maximumValue() : 0;
}
