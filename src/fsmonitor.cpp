#include "fsmonitor.h"
#include <qmusbmode.h>

FSMonitor::FSMonitor(QObject *parent) :
  QObject(parent), m_mode(new MeeGo::QmUSBMode(this)) {

  QObject::connect(m_mode, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)),
		   this, SLOT(modeChanged()));

  m_available =
    m_mode->mountStatus(MeeGo::QmUSBMode::DocumentDirectoryMount)
    .testFlag(MeeGo::QmUSBMode::ReadWriteMount);
}

FSMonitor::~FSMonitor() {

}

bool FSMonitor::isAvailable() const {
  return m_available;
}

void FSMonitor::setAvailable(bool available) {
  if (m_available != available) {
    m_available = available;
    emit availabilityChanged();
  }
}

void FSMonitor::modeChanged() {
  setAvailable(m_mode->mountStatus(MeeGo::QmUSBMode::DocumentDirectoryMount)
	       .testFlag(MeeGo::QmUSBMode::ReadWriteMount));
}
