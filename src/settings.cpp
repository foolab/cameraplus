/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2014 Mohammed Sameer <msameer@foolab.org>
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

#ifdef SAILFISH
#define PATH QString("%1%2.config%2harbour-cameraplus%2harbour-cameraplus.conf").arg(QDir::homePath()).arg(QDir::separator())
#else
#define PATH QString("%1%2.config%2cameraplus.conf").arg(QDir::homePath()).arg(QDir::separator())
#endif

#define DEFAULT_MODE                             1
#define DEFAULT_USE_GPS                          true
#define DEFAULT_USE_GEOTAGS                      true
#define DEFAULT_SOUND_ENABLED                    true
#define DEFAULT_SHOW_TOOL_BAR                    false
#define DEFAULT_VIDEO_MUTE                       false
#define DEFAULT_GRID_ENABLED                     false
#define DEFAULT_FACE_DETECTION_ENABLED           true
#define DEFAULT_ZOOM_AS_SHUTTER                  false
#define DEFAULT_PROXIMITY_AS_SHUTTER             false
#define DEFAULT_DEVICE                           0
#define DEFAULT_ENABLE_PREVIEW                   true
#define DEFAULT_NIGHT_MODE                       false
#define DEFAULT_PLUGIN                           "org.foolab.cameraplus.image"
#define DEFAULT_CAPTURE_TIMER_DELAY              5
#define DEFAULT_LEFT_HANDED_MODE                 false
#define DEFAULT_SEQUENTIAL_SHOTS_COUNT           5
#define DEFAULT_SEQUENTIAL_SHOTS_INTERVAL        5
#define DEFAULT_SEQUENTIAL_SHOTS_DELAY           0
#define DEFAULT_SEQUENTIAL_SHOTS_FOCUS           true
#define DEFAULT_PANORAMA_KEEP_FRAMES             false
#define DEFAULT_PANORAMA_USE_HIGH_RESOLUTION     false
#define DEFAULT_FILE_NAMING_IN_UTC               true

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
    m_settings->setValue("camera/device", device);
    emit deviceChanged();
  }
}

QVariant Settings::value(const QString& key, const QVariant& defaultValue) const {
  return m_settings->value(key, defaultValue);
}

void Settings::setValue(const QString& key, const QVariant& value)  {
  m_settings->setValue(key, value);
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

QString Settings::plugin() const {
  return m_settings->value("camera/plugin", DEFAULT_PLUGIN).toString();
}

void Settings::setPlugin(const QString& plugin) {
  if (Settings::plugin() != plugin) {
    m_settings->setValue("camera/plugin", plugin);
    emit pluginChanged();
  }
}

int Settings::captureTimerDelay() const {
  return m_settings->value("captureTimer/delay", DEFAULT_CAPTURE_TIMER_DELAY).toInt();
}

void Settings::setCaptureTimerDelay(int delay) {
  if (delay != captureTimerDelay()) {
    m_settings->setValue("captureTimer/delay", delay);
    emit captureTimerDelayChanged();
  }
}

bool Settings::isLeftHandedModeEnabled() {
  return m_settings->value("camera/leftHandedMode", DEFAULT_LEFT_HANDED_MODE).toBool();
}

void Settings::setLeftHandedModeEnabled(bool enabled) {
  if (isLeftHandedModeEnabled() != enabled) {
    m_settings->setValue("camera/leftHandedMode", enabled);

    emit leftHandedModeChanged();
  }
}

int Settings::sequentialShotsCount() const {
  return m_settings->value("sequentialShots/count", DEFAULT_SEQUENTIAL_SHOTS_COUNT).toInt();
}

void Settings::setSequentialShotsCount(int count) {
  if (sequentialShotsCount() != count) {
    m_settings->setValue("sequentialShots/count", count);
    emit sequentialShotsCountChanged();
  }
}

int Settings::sequentialShotsInterval() const {
  return m_settings->value("sequentialShots/interval", DEFAULT_SEQUENTIAL_SHOTS_INTERVAL).toInt();
}

void Settings::setSequentialShotsInterval(int interval) {
  if (sequentialShotsInterval() != interval) {
    m_settings->setValue("sequentialShots/interval", interval);
    emit sequentialShotsIntervalChanged();
  }
}

int Settings::sequentialShotsDelay() const {
  return m_settings->value("sequentialShots/delay", DEFAULT_SEQUENTIAL_SHOTS_DELAY).toInt();
}

void Settings::setSequentialShotsDelay(int delay) {
  if (sequentialShotsDelay() != delay) {
    m_settings->setValue("sequentialShots/delay", delay);
    emit sequentialShotsDelayChanged();
  }
}

bool Settings::isFocusBeforeSequentialShotsEnabled() const {
  return m_settings->value("sequentialShots/focus", DEFAULT_SEQUENTIAL_SHOTS_FOCUS).toBool();
}

void Settings::setFocusBeforeSequentialShotsEnabled(bool enabled) {
  if (isFocusBeforeSequentialShotsEnabled() != enabled) {
    m_settings->setValue("sequentialShots/focus", enabled);
    emit focusBeforeSequentialShotsChanged();
  }
}

bool Settings::panoramaKeepFrames() const {
  return m_settings->value("panorama/keepFrames", DEFAULT_PANORAMA_KEEP_FRAMES).toBool();
}

void Settings::setPanoramaKeepFrames(bool keep) {
  if (panoramaKeepFrames() != keep) {
    m_settings->setValue("panorama/keepFrames", keep);
    emit panoramaKeepFramesChanged();
  }
}

bool Settings::panoramaUseHighResolution() const {
  return m_settings->value("panorama/useHighResolution", DEFAULT_PANORAMA_USE_HIGH_RESOLUTION).toBool();
}

void Settings::setPanoramaUseHighResolution(bool use) {
  if (panoramaUseHighResolution() != use) {
    m_settings->setValue("panorama/useHighResolution", use);
    emit panoramaUseHighResolutionChanged();
  }
}

bool Settings::isFileNamingInUtc() const {
  return m_settings->value("fileNaming/inUtc", DEFAULT_FILE_NAMING_IN_UTC).toBool();
}

void Settings::setFileNamingInUtc(bool inUtc) {
  if (isFileNamingInUtc() != inUtc) {
    m_settings->setValue("fileNaming/inUtc", inUtc);

    emit fileNamingInUtcChanged();
  }
}
