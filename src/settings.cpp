#include "settings.h"
#include <QSettings>
#include <QDir>

#define PATH QString("%1%2.config%2/cameraplus.conf").arg(QDir::homePath()).arg(QDir::separator())

#define DEFAULT_MODE          0
#define DEFAULT_SCENE_MODE    6 // Auto
#define DEFAULT_TIMEOUT       0
#define DEFAULT_USE_GPS       true
#define DEFAULT_USE_GEOTAGS   true
#define DEFAULT_COLOR_FILTER  0
#define DEFAULT_WHITE_BALANCE 0
#define DEFAULT_EV_COMP       0.0
#define DEFAULT_FLASH_MODE    0

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

QString Settings::creatorName() const {
  return m_settings->value("camera/creatorName").toString();
}

void Settings::setCreatorName(const QString& name) {
  if (name != creatorName()) {
    m_settings->setValue("camera/creatorName", name);

    emit creatorNameChanged();
  }
}

int Settings::postCaptureTimeout() const {
  return m_settings->value("camera/postCaptureTimeout", DEFAULT_TIMEOUT).toInt();
}

void Settings::setPostCaptureTimeout(int timeout) {
  if (timeout != postCaptureTimeout()) {
    m_settings->setValue("camera/postCaptureTimeout", timeout);

    emit postCaptureTimeoutChanged();
  }
}

bool Settings::useGps() const {
  return m_settings->value("camera/useGps", DEFAULT_USE_GPS).toBool();
}

void Settings::setUseGps(bool enable) {
  if (enable == useGps()) {
    m_settings->setValue("camera/useGps", enable);

    emit useGpsChanged();
  }
}

bool Settings::useGeotags() const {
  return m_settings->value("camera/useGeotags", DEFAULT_USE_GEOTAGS).toBool();
}

void Settings::setUseGeotags(bool enable) {
  if (enable != useGeotags()) {
    m_settings->setValue("camera/useGeotags", enable);

    emit useGeotagsChanged();
  }
}

int Settings::imageSceneMode() const {
  return m_settings->value("image/sceneMode", DEFAULT_SCENE_MODE).toInt();
}

void Settings::setImageSceneMode(int mode) {
  if (mode != imageSceneMode()) {
    m_settings->setValue("image/sceneMode", mode);

    emit imageSceneModeChanged();
  }
}

int Settings::imageColorFilter() const {
  return m_settings->value("image/colorFilter", DEFAULT_COLOR_FILTER).toInt();
}

void Settings::setImageColorFilter(int filter) {
  if (filter != imageColorFilter()) {
    m_settings->setValue("image/colorFilter", filter);

    emit imageColorFilterChanged();
  }
}

int Settings::imageWhiteBalance() const {
  return m_settings->value("image/whiteBalance", DEFAULT_WHITE_BALANCE).toInt();
}

void Settings::setImageWhiteBalance(int wb) {
  if (wb != imageWhiteBalance()) {
    m_settings->setValue("image/whiteBalance", wb);

    emit imageWhiteBalanceChanged();
  }
}

qreal Settings::imageEvComp() const {
  return m_settings->value("image/evComp", DEFAULT_EV_COMP).toReal();
}

void Settings::setImageEvComp(qreal ev) {
  if (ev != imageEvComp()) {
    m_settings->setValue("image/evComp", ev);

    emit imageEvCompChanged();
  }
}

int Settings::videoSceneMode() const {
  return m_settings->value("video/sceneMode", DEFAULT_SCENE_MODE).toInt();
}

void Settings::setVideoSceneMode(int mode) {
  if (mode != videoSceneMode()) {
    m_settings->setValue("video/sceneMode", mode);

    emit videoSceneModeChanged();
  }
}

int Settings::videoColorFilter() const {
  return m_settings->value("video/colorFilter", DEFAULT_COLOR_FILTER).toInt();
}

void Settings::setVideoColorFilter(int filter) {
  if (filter != videoColorFilter()) {
    m_settings->setValue("video/colorFilter", filter);

    emit videoColorFilterChanged();
  }
}

int Settings::videoWhiteBalance() const {
  return m_settings->value("video/whiteBalance", DEFAULT_WHITE_BALANCE).toInt();
}

void Settings::setVideoWhiteBalance(int wb) {
  if (wb != videoWhiteBalance()) {
    m_settings->setValue("video/whiteBalance", wb);

    emit videoWhiteBalanceChanged();
  }
}

qreal Settings::videoEvComp() const {
  return m_settings->value("video/evComp", DEFAULT_EV_COMP).toReal();
}

void Settings::setVideoEvComp(qreal ev) {
  if (ev != videoEvComp()) {
    m_settings->setValue("video/evComp", ev);

    emit videoEvCompChanged();
  }
}

int Settings::imageFlashMode() const {
  return m_settings->value("image/flashMode", DEFAULT_FLASH_MODE).toInt();
}

void Settings::setImageFlashMode(int mode) {
  if (mode != imageFlashMode()) {
    m_settings->setValue("image/flashMode", mode);

    emit imageFlashModeChanged();
  }
}
