#include "videotorch.h"
#include "qtcamvideotorch.h"
#include "camera.h"
#include "qtcamdevice.h"
#include "qtcamvideomode.h"

VideoTorch::VideoTorch(QObject *parent) :
  QObject(parent), m_cam(0), m_torch(0) {

}

VideoTorch::~VideoTorch() {
  m_cam = 0;
}

Camera *VideoTorch::camera() {
  return m_cam;
}

void VideoTorch::setCamera(Camera *camera) {
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

bool VideoTorch::isOn() const {
  return m_torch ? m_torch->isOn() : false;
}

void VideoTorch::setOn(bool on) {
  if (m_torch) {
    m_torch->setOn(on);
  }
}

void VideoTorch::deviceChanged() {
  if (m_torch) {
    delete m_torch; m_torch = 0;
  }

  if (m_cam->device()) {
    m_torch = new QtCamVideoTorch(m_cam->device(), this);
    QObject::connect(m_torch, SIGNAL(stateChanged()), this, SIGNAL(stateChanged()));

    emit stateChanged();
  }
}
