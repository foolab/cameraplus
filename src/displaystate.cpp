#include "displaystate.h"
#include <qmsystem2/qmdisplaystate.h>
#include <QTimer>
#include <QDebug>

DisplayState::DisplayState(QObject *parent) :
  QObject(parent), m_state(new MeeGo::QmDisplayState(this)), m_timer(new QTimer(this)) {

  m_timer->setSingleShot(false);
  m_timer->setInterval(50 * 1000);

  QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

DisplayState::~DisplayState() {
  setInhibitDim(false);
}

bool DisplayState::isDimInhibited() const {
  return m_timer->isActive();
}

void DisplayState::setInhibitDim(bool inhibit) {
  if (m_timer->isActive() == inhibit) {
    return;
  }

  if (!inhibit) {
    if (!m_state->cancelBlankingPause()) {
      qWarning() << "Failed to cancel display dimming!";
    }

    m_timer->stop();
  }
  else {
    if (!m_state->setBlankingPause()) {
      qWarning() << "Failed to inhibit display dimming!";
      return;
    }

    m_timer->start();
  }

  emit inhibitDimChanged();
}

void DisplayState::timeout() {
  if (!m_state->setBlankingPause()) {
    qWarning() << "Failed to inhibit display dimming!";
  }
}
