#include "capability.h"
#include "camera.h"

Capability::Capability(QObject *parent) :
  QObject(parent),
  m_cam(0) {

}

Capability::~Capability() {

}

Camera *Capability::camera() {
  return m_cam;
}

void Capability::setCamera(Camera *cam) {
  if (cam == m_cam) {
    return;
  }

  if (m_cam) {
    QObject::disconnect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
  }

  m_cam = cam;

  if (m_cam) {
    QObject::connect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
  }

  emit cameraChanged();

  deviceChanged();
}
