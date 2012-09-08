#include "camera.h"
#include "qtcamera.h"
#include "qtcamdevice.h"
#include "qtcammode.h"
#include "qtcamimagemode.h"
#include "qtcamvideomode.h"
#include "qtcamgraphicsviewfinder.h"

Camera::Camera(QDeclarativeItem *parent) :
  QDeclarativeItem(parent),
  m_cam(new QtCamera(this)),
  m_dev(0),
  m_vf(new QtCamGraphicsViewfinder(m_cam->config(), this)),
  m_mode(Camera::ImageMode) {

  // TODO:
}

Camera::~Camera() {
  // TODO:
}

void Camera::componentComplete() {
  QDeclarativeItem::componentComplete();

  if (m_id.isValid()) {
    QVariant oldId = m_id;
    m_id = QVariant();
    setDeviceId(oldId);
  }

  emit deviceCountChanged();
}

int Camera::deviceCount() const {
  return m_cam ? m_cam->devices().size() : 0;
}

QString Camera::deviceName(int index) const {
  return m_cam->devices().at(index).first;
}

QVariant Camera::deviceId(int index) const {
  return m_cam->devices().at(index).second;
}

void Camera::setDeviceId(const QVariant& id) {
  if (id == m_id) {
    return;
  }

  if (!isComponentComplete()) {
    m_id = id;
    emit deviceIdChanged();
    return;
  }

  if (m_dev && m_dev->stop()) {
    delete m_dev;
  }
  else if (m_dev) {
    qWarning() << "Failed to stop device";
    return;
  }

  m_dev = m_cam->device(id, this);

  m_id = id;

  m_vf->setDevice(m_dev);

  if (m_mode == Camera::VideoMode) {
    m_dev->videoMode()->activate();
  }
  else {
    m_dev->imageMode()->activate();
  }

  QObject::connect(m_dev, SIGNAL(runningStateChanged(bool)),
		      this, SIGNAL(runningStateChanged()));
  QObject::connect(m_dev, SIGNAL(idleStateChanged(bool)), this, SIGNAL(idleStateChanged()));

  emit deviceIdChanged();
  emit deviceChanged();
}

QVariant Camera::deviceId() const {
  return m_id;
}

void Camera::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);

  m_vf->setGeometry(newGeometry);
}

QtCamDevice *Camera::device() const {
  return m_dev;
}

void Camera::setMode(const Camera::CameraMode& mode) {
  m_mode = mode;

  if (!m_dev) {
    return;
  }

  Camera::CameraMode current = m_dev->activeMode() == m_dev->videoMode() ?
    Camera::VideoMode : Camera::ImageMode;

  if (current == mode) {
    return;
  }

  if (mode == Camera::VideoMode) {
    m_dev->videoMode()->activate();
  }
  else {
    m_dev->imageMode()->activate();
  }

  emit modeChanged();
}

Camera::CameraMode Camera::mode() {
  return m_mode;
}

bool Camera::start() {
  return m_dev ? m_dev->start() : false;
}

void Camera::stop() {
  if (m_dev) {
    m_dev->stop();
  }
}

bool Camera::isIdle() {
  return m_dev ? m_dev->isIdle() : true;
}

bool Camera::isRunning() {
  return m_dev ? m_dev->isRunning() : false;
}
