#include "settings.h"
#include <QSettings>
#include <QDir>

#define PATH QString("%1%2.config%2/cameraplus.conf").arg(QDir::homePath()).arg(QDir::separator())

#define DEFAULT_MODE 0

Settings::Settings(QObject *parent) :
  QObject(parent),
  m_settings(new QSettings(PATH, QSettings::IniFormat, this)) {

}

Settings::~Settings() {
  delete m_settings; m_settings = 0;
}

int Settings::mode() const {
  return m_settings->value("camera/mode", DEFAULT_MODE).toInt();
}

void Settings::setMode(int mode) {
  if (mode != Settings::mode()) {
    m_settings->setValue("camera/mode", mode);
    emit modeChanged();
  }
}

