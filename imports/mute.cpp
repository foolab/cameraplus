#include "mute.h"
#include "qtcammute.h"
#include "camera.h"
#include "qtcamdevice.h"

Mute::Mute(QObject *parent) :
  QObject(parent), m_cam(0), m_mute(0) {

}

Mute::~Mute() {
  m_cam = 0;
}

Camera *Mute::camera() {
  return m_cam;
}

void Mute::setCamera(Camera *camera) {
  if (camera == m_cam) {
    return;
  }

  if (m_cam) {
    QObject::disconnect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
  }

  m_cam = camera;

  if (m_cam) {
    QObject::connect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
  }

  emit cameraChanged();

  deviceChanged();
}

bool Mute::isEnabled() const {
  return m_mute ? m_mute->isEnabled() : false;
}

void Mute::setEnabled(bool enabled) {
  if (m_mute) {
    m_mute->setEnabled(enabled);
  }
}

void Mute::deviceChanged() {
  if (m_mute) {
    delete m_mute; m_mute = 0;
  }

  if (m_cam->device()) {
    m_mute = new QtCamMute(m_cam->device(), this);
    QObject::connect(m_mute, SIGNAL(stateChanged()), this, SIGNAL(stateChanged()));

    emit stateChanged();
  }
}
