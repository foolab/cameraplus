#include "orientation.h"
#include <qmorientation.h>
#include <QDebug>

Orientation::Orientation(QObject *parent) :
  QObject(parent),
  m_orientation(new MeeGo::QmOrientation(this)),
  m_direction(Unknown) {

  QObject::connect(m_orientation, SIGNAL(orientationChanged(const MeeGo::QmOrientationReading&)),
		   this, SLOT(orientationChanged(const MeeGo::QmOrientationReading&)));

  if (m_orientation->requestSession(MeeGo::QmSensor::SessionTypeListen)
      == MeeGo::QmSensor::SessionTypeNone) {
    qDebug() << "Failed to get listen session:" << m_orientation->lastError();
  }
}

Orientation::~Orientation() {
  m_orientation->stop();
}

bool Orientation::isActive() const {
  return m_orientation->isRunning();
}

void Orientation::setActive(bool active) {
  if (active == isActive()) {
    return;
  }

  if (active) {
    m_orientation->start();
    orientationChanged(m_orientation->orientation());
  }
  else {
    m_orientation->stop();
    m_direction = Unknown;

    emit orientationChanged();
  }

  emit activeChanged();
}

Orientation::OrientationDirection Orientation::orientation() const {
  return m_direction;
}

void Orientation::orientationChanged(const MeeGo::QmOrientationReading& value) {
  OrientationDirection direction = Unknown;

  switch (value.value) {
  case MeeGo::QmOrientation::BottomUp:
    direction = InvertedLandscape;
    break;

  case MeeGo::QmOrientation::BottomDown:
    direction = Landscape;
    break;

  case MeeGo::QmOrientation::LeftUp:
    direction = Portrait;
    break;

  case MeeGo::QmOrientation::RightUp:
    direction = InvertedPortrait;
    break;

  default:
    direction = Unknown;
    break;
  }

  if (direction != m_direction) {
    m_direction = direction;
    emit orientationChanged();
  }
}
