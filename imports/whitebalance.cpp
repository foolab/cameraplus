#include "whitebalance.h"
#include "camera.h"
#include <QDebug>

WhiteBalance::WhiteBalance(QObject *parent) :
  Capability(parent),
  m_wb(0) {

}

WhiteBalance::~WhiteBalance() {
  if (m_wb) {
    delete m_wb; m_wb = 0;
  }
}

void WhiteBalance::deviceChanged() {
  if (m_wb) {
    delete m_wb; m_wb = 0;
  }

  if (m_cam->device()) {
    m_wb = new QtCamWhiteBalance(m_cam->device(), this);
    QObject::connect(m_wb, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
  }

  emit valueChanged();
}

WhiteBalance::WhiteBalanceMode WhiteBalance::value() {
  return m_wb ? (WhiteBalanceMode)m_wb->value() : WhiteBalance::Auto;
}

void WhiteBalance::setValue(const WhiteBalance::WhiteBalanceMode& mode) {
  if (m_wb) {
    m_wb->setValue((QtCamWhiteBalance::WhiteBalanceMode)mode);
  }
}
