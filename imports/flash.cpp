#include "flash.h"
#include "camera.h"

Flash::Flash(QObject *parent) :
  Capability(parent),
  m_flash(0) {

}

Flash::~Flash() {
  if (m_flash) {
    delete m_flash; m_flash = 0;
  }
}

void Flash::deviceChanged() {
  if (m_flash) {
    delete m_flash; m_flash = 0;
  }

  if (m_cam->device()) {
    m_flash = new QtCamFlash(m_cam->device(), this);
    QObject::connect(m_flash, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
  }

  emit valueChanged();
}

Flash::FlashMode Flash::value() {
  return m_flash ? (FlashMode)m_flash->value() : Flash::Auto;
}

void Flash::setValue(const Flash::FlashMode& mode) {
  if (m_flash) {
    m_flash->setValue((QtCamFlash::FlashMode)mode);
  }
}
