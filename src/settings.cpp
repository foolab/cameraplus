/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "settings.h"
#include <QSettings>
#include <QDir>
#include "devicesettings.h"

#define PATH QString("%1%2.config%2/cameraplus.conf").arg(QDir::homePath()).arg(QDir::separator())

#define DEFAULT_MODE                    1
#define DEFAULT_USE_GPS                 true
#define DEFAULT_USE_GEOTAGS             true
#define DEFAULT_SOUND_ENABLED           true
#define DEFAULT_SHOW_TOOL_BAR           false
#define DEFAULT_VIDEO_MUTE              false
#define DEFAULT_GRID_ENABLED            false
#define DEFAULT_FACE_DETECTION_ENABLED  true
#define DEFAULT_ZOOM_AS_SHUTTER         false
#define DEFAULT_PROXIMITY_AS_SHUTTER    false
#define DEFAULT_DEVICE                  0
#define DEFAULT_ENABLE_PREVIEW          true
#define DEFAULT_NIGHT_MODE              false

Settings::Settings(QObject *parent) :
  QObject(parent),
  m_settings(new QSettings(PATH, QSettings::IniFormat, this)),
  m_device(0) {

}

Settings::~Settings() {
  delete m_settings; m_settings = 0;
  delete m_device; m_device = 0;
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

bool Settings::useGps() const {
  return m_settings->value("camera/useGps", DEFAULT_USE_GPS).toBool();
}

void Settings::setUseGps(bool enable) {
  if (enable != useGps()) {
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

bool Settings::isSoundEnabled() const {
  return m_settings->value("camera/soundEnabled", DEFAULT_SOUND_ENABLED).toBool();
}

void Settings::setSoundEnabled(bool enabled) {
  if (isSoundEnabled() != enabled) {
    m_settings->setValue("camera/soundEnabled", enabled);
    emit soundEnabledChanged();
  }
}

bool Settings::isToolBarShown() const {
  return m_settings->value("camera/showToolBar", DEFAULT_SHOW_TOOL_BAR).toBool();
}

void Settings::setToolBarShown(bool shown) {
  if (isToolBarShown() != shown) {
    m_settings->setValue("camera/showToolBar", shown);

    emit toolBarShownChanged();
  }
}

bool Settings::isVideoMuted() const {
  return m_settings->value("video/mute", DEFAULT_VIDEO_MUTE).toBool();
}

void Settings::setVideoMuted(bool muted) {
  if (isVideoMuted() != muted) {
    m_settings->setValue("video/mute", muted);
    emit videoMutedChanged();
  }
}

bool Settings::isGridEnabled() const {
  return m_settings->value("camera/gridEnabled", DEFAULT_GRID_ENABLED).toBool();
}

void Settings::setGridEnabled(bool enabled) {
  if (enabled != isGridEnabled()) {
    m_settings->setValue("camera/gridEnabled", enabled);
    emit gridEnabledChanged();
  }
}

bool Settings::isFaceDetectionEnabled() const {
  return m_settings->value("image/faceDetectionEnabled", DEFAULT_FACE_DETECTION_ENABLED).toBool();
}

void Settings::setFaceDetectionEnabled(bool enabled) {
  if (isFaceDetectionEnabled() != enabled) {
    m_settings->setValue("image/faceDetectionEnabled", enabled);
    emit faceDetectionEnabledChanged();
  }
}

bool Settings::isZoomAsShutterEnabled() const {
  return m_settings->value("camera/zoomAsShutter", DEFAULT_ZOOM_AS_SHUTTER).toBool();
}

void Settings::setZoomAsShutterEnabled(bool enabled) {
  if (isZoomAsShutterEnabled() != enabled) {
    m_settings->setValue("camera/zoomAsShutter", enabled);

    emit zoomAsShutterChanged();
  }
}

bool Settings::isProximityAsShutterEnabled() const {
  return m_settings->value("camera/proximityAsShutter", DEFAULT_PROXIMITY_AS_SHUTTER).toBool();
}

void Settings::setProximityAsShutterEnabled(bool enabled) {
  if (isProximityAsShutterEnabled() != enabled) {
    m_settings->setValue("camera/proximityAsShutter", enabled);

    emit proximityAsShutterChanged();
  }
}

int Settings::device() const {
  return m_settings->value("camera/device", DEFAULT_DEVICE).toInt();
}

void Settings::setDevice(int device) {
  if (device != Settings::device()) {
    emit deviceAboutToChange();

    m_settings->setValue("camera/device", device);

    delete m_device; m_device = 0;

    if (mode() == DEFAULT_MODE) {
      // image
      emit imageSceneModeChanged();
      emit imageColorFilterChanged();
      emit imageWhiteBalanceChanged();
      emit imageEvCompChanged();
      emit imageFlashModeChanged();
      emit imageIsoChanged();
      emit imageAspectRatioChanged();
      emit imageResolutionChanged();
    }
    else {
      // video
      emit videoSceneModeChanged();
      emit videoColorFilterChanged();
      emit videoWhiteBalanceChanged();
      emit videoEvCompChanged();
      emit videoAspectRatioChanged();
      emit videoResolutionChanged();
      emit videoTorchOnChanged();
    }

    emit deviceChanged();
  }
}

DeviceSettings *Settings::deviceSettings() {
  if (m_device) {
    return m_device;
  }

  int device = Settings::device();

  if (device == 0) {
    m_device = new PrimaryDeviceSettings;
  }
  else {
    m_device = new SecondaryDeviceSettings;
  }

  return m_device;
}

QVariant Settings::deviceValue(const char *key, const QVariant& defaultValue) {
  QString k = QString("%1/%2").arg(deviceSettings()->id()).arg(key);

  return m_settings->value(k, defaultValue);
}

void Settings::setDeviceValue(const char *key, const QVariant& value) {
  QString k = QString("%1/%2").arg(deviceSettings()->id()).arg(key);

  m_settings->setValue(k, value);
}

// Device dependant settings

int Settings::imageSceneMode() {
  return deviceValue("image/sceneMode", deviceSettings()->defaultImageSceneMode()).toInt();
}

void Settings::setImageSceneMode(int mode) {
  if (mode != imageSceneMode()) {
    setDeviceValue("image/sceneMode", mode);
  }

  // We always emit the signal to reset scene and all scene associated values
  emit imageSceneModeChanged();
}

int Settings::imageColorFilter() {
  return deviceValue("image/colorFilter", deviceSettings()->defaultImageColorFilter()).toInt();
}

void Settings::setImageColorFilter(int filter) {
  if (filter != imageColorFilter()) {
    setDeviceValue("image/colorFilter", filter);

    emit imageColorFilterChanged();
  }
}

int Settings::imageWhiteBalance() {
  return deviceValue("image/whiteBalance", deviceSettings()->defaultImageWhiteBalance()).toInt();
}

void Settings::setImageWhiteBalance(int wb) {
  if (wb != imageWhiteBalance()) {
    setDeviceValue("image/whiteBalance", wb);

    emit imageWhiteBalanceChanged();
  }
}

qreal Settings::imageEvComp() {
  return deviceValue("image/evComp", deviceSettings()->defaultImageEvComp()).toReal();
}

void Settings::setImageEvComp(qreal ev) {
  if (!qFuzzyCompare(ev, imageEvComp())) {
    setDeviceValue("image/evComp", ev);

    emit imageEvCompChanged();
  }
}

int Settings::videoSceneMode() {
  return deviceValue("video/sceneMode", deviceSettings()->defaultVideoSceneMode()).toInt();
}

void Settings::setVideoSceneMode(int mode) {
  if (mode != videoSceneMode()) {
    setDeviceValue("video/sceneMode", mode);
  }

  emit videoSceneModeChanged();
}

int Settings::videoColorFilter() {
  return deviceValue("video/colorFilter", deviceSettings()->defaultVideoColorFilter()).toInt();
}

void Settings::setVideoColorFilter(int filter) {
  if (filter != videoColorFilter()) {
    setDeviceValue("video/colorFilter", filter);

    emit videoColorFilterChanged();
  }
}

int Settings::videoWhiteBalance() {
  return deviceValue("video/whiteBalance", deviceSettings()->defaultVideoWhiteBalance()).toInt();
}

void Settings::setVideoWhiteBalance(int wb) {
  if (wb != videoWhiteBalance()) {
    setDeviceValue("video/whiteBalance", wb);

    emit videoWhiteBalanceChanged();
  }
}

qreal Settings::videoEvComp() {
  return deviceValue("video/evComp", deviceSettings()->defaultVideoEvComp()).toReal();
}

void Settings::setVideoEvComp(qreal ev) {
  if (!qFuzzyCompare(ev, videoEvComp())) {
    setDeviceValue("video/evComp", ev);

    emit videoEvCompChanged();
  }
}

int Settings::imageFlashMode() {
  return deviceValue("image/flashMode", deviceSettings()->defaultImageFlashMode()).toInt();
}

void Settings::setImageFlashMode(int mode) {
  if (mode != imageFlashMode()) {
    setDeviceValue("image/flashMode", mode);

    emit imageFlashModeChanged();
  }
}

int Settings::imageIso() {
  return deviceValue("image/iso", deviceSettings()->defaultImageIso()).toInt();
}

void Settings::setImageIso(int iso) {
  if (imageIso() != iso) {
    setDeviceValue("image/iso", iso);
    emit imageIsoChanged();
  }
}

QString Settings::imageAspectRatio() {
  return deviceValue("image/aspectRatio", deviceSettings()->defaultImageAspectRatio()).toString();
}

void Settings::setImageAspectRatio(const QString& aspectRatio) {
  if (aspectRatio != imageAspectRatio()) {
    setDeviceValue("image/aspectRatio", aspectRatio);
    emit imageAspectRatioChanged();
  }
}

QString Settings::imageResolution() {
  return deviceValue("image/resolution", deviceSettings()->defaultImageResolution()).toString();
}

void Settings::setImageResolution(const QString& resolution) {
  if (resolution != imageResolution()) {
    setDeviceValue("image/resolution", resolution);
    emit imageResolutionChanged();
  }
}

QString Settings::videoAspectRatio() {
  return deviceValue("video/aspectRatio", deviceSettings()->defaultVideoAspectRatio()).toString();
}


void Settings::setVideoAspectRatio(const QString& aspectRatio) {
  if (Settings::videoAspectRatio() != aspectRatio) {
    setDeviceValue("video/aspectRatio", aspectRatio);
    emit videoAspectRatioChanged();
  }
}

QString Settings::videoResolution() {
  return deviceValue("video/resolution", deviceSettings()->defaultVideoResolution()).toString();
}

void Settings::setVideoResolution(const QString& resolution) {
  if (resolution != videoResolution()) {
    setDeviceValue("video/resolution", resolution);
    emit videoResolutionChanged();
  }
}

bool Settings::isVideoTorchOn() {
  return deviceValue("video/torchOn", deviceSettings()->defaultVideoTorchOn()).toBool();
}

void Settings::setVideoTorchOn(bool on) {
  if (isVideoTorchOn() != on) {
    setDeviceValue("video/torchOn", on);
    emit videoTorchOnChanged();
  }
}

QString Settings::fileNamingStamp(const QString& id) const {
  QString key = QString("fileNaming/%1").arg(id);
  return m_settings->value(key).toString();
}

void Settings::setFileNamingStamp(const QString& id, const QString& stamp) {
  QString key = QString("fileNaming/%1").arg(id);
  m_settings->setValue(key, stamp);
}

int Settings::fileNamingCounter(const QString& id) const {
  QString key = QString("fileNaming/%1").arg(id);
  return m_settings->value(key).toInt();
}

void Settings::setFileNamingCounter(const QString& id, int counter) {
  QString key = QString("fileNaming/%1").arg(id);
  m_settings->setValue(key, counter);
}

bool Settings::isPreviewEnabled() const {
  return m_settings->value("camera/enablePreview", DEFAULT_ENABLE_PREVIEW).toBool();
}

void Settings::setPreviewEnabled(bool enabled) {
  if (enabled != isPreviewEnabled()) {
    m_settings->setValue("camera/enablePreview", enabled);

    emit previewEnabledChanged();
  }
}

bool Settings::isNightModeEnabled() const {
  return m_settings->value("camera/nightMode", DEFAULT_NIGHT_MODE).toBool();
}

void Settings::setNightModeEnabled(bool enabled) {
  if (isNightModeEnabled() != enabled) {
    m_settings->setValue("camera/nightMode", enabled);
    emit nightModeChanged();
  }
}
