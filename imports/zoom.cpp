#include "zoom.h"
#include "camera.h"
#include "qtcamzoom.h"

Zoom::Zoom(QObject *parent) :
  Capability(parent),
  m_zoom(0) {

}

Zoom::~Zoom() {
  if (m_zoom) {
    delete m_zoom; m_zoom = 0;
  }
}

void Zoom::deviceChanged() {
  if (m_zoom) {
    delete m_zoom; m_zoom = 0;
  }

  if (m_cam->device()) {
    m_zoom = new QtCamZoom(m_cam->device(), this);
    QObject::connect(m_zoom, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
    QObject::connect(m_zoom, SIGNAL(minimumValueChanged()), this, SIGNAL(minimumChanged()));
    QObject::connect(m_zoom, SIGNAL(maximumValueChanged()), this, SIGNAL(maximunmChanged()));
  }

  emit valueChanged();
  emit minimumChanged();
  emit maximunmChanged();
}

qreal Zoom::value() {
  return m_zoom ? m_zoom->value() : 1.0;
}

void Zoom::setValue(qreal val) {
  if (m_zoom) {
    m_zoom->setValue(val);
  }
}

qreal Zoom::minimum() {
  return m_zoom ? m_zoom->minimumValue() : 1.0;
}

qreal Zoom::maximum() {
  return m_zoom ? m_zoom->maximumValue() : 1.0;
}


