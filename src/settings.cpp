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

#define PATH QString("%1%2.config%2/cameraplus.conf").arg(QDir::homePath()).arg(QDir::separator())

#define DEFAULT_MODE                    1
#define DEFAULT_SCENE_MODE              6 // Auto
#define DEFAULT_USE_GPS                 true
#define DEFAULT_USE_GEOTAGS             true
#define DEFAULT_COLOR_FILTER            0
#define DEFAULT_WHITE_BALANCE           0
#define DEFAULT_EV_COMP                 0.0
#define DEFAULT_FLASH_MODE              0
#define DEFAULT_IMAGE_ISO               0
#define DEFAULT_IMAGE_RESOLUTION        "high"
#define DEFAULT_IMAGE_ASPECT_RATIO      "16:9"
#define DEFAULT_VIDEO_RESOLUTION        "high"
#define DEFAULT_SOUND_ENABLED           true
#define DEFAULT_VIDEO_TORCH_ON          false
#define DEFAULT_SHOW_TOOL_BAR           false
#define DEFAULT_VIDEO_MUTE              false
#define DEFAULT_GRID_ENABLED            false
#define DEFAULT_FACE_DETECTION_ENABLED  true
#define DEFAULT_ZOOM_AS_SHUTTER         false
#define DEFAULT_DEVICE                  0

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

int Settings::imageSceneMode() const {
  return m_settings->value("image/sceneMode", DEFAULT_SCENE_MODE).toInt();
}

void Settings::setImageSceneMode(int mode) {
  if (mode != imageSceneMode()) {
    m_settings->setValue("image/sceneMode", mode);
  }

  emit imageSceneModeChanged();
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
  if (!qFuzzyCompare(ev, imageEvComp())) {
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
  }

  emit videoSceneModeChanged();
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
  if (!qFuzzyCompare(ev, videoEvComp())) {
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

int Settings::imageIso() const {
  return m_settings->value("image/iso", DEFAULT_IMAGE_ISO).toInt();
}

void Settings::setImageIso(int iso) {
  if (imageIso() != iso) {
    m_settings->setValue("image/iso", iso);
    emit imageIsoChanged();
  }
}

QString Settings::imageAspectRatio() const {
  return m_settings->value("image/aspectRatio", DEFAULT_IMAGE_ASPECT_RATIO).toString();
}

void Settings::setImageAspectRatio(const QString& aspectRatio) {
  if (aspectRatio != imageAspectRatio()) {
    m_settings->setValue("image/aspectRatio", aspectRatio);
    emit imageAspectRatioChanged();
  }
}

QString Settings::imageResolution() const {
  return m_settings->value("image/resolution", DEFAULT_IMAGE_RESOLUTION).toString();
}

void Settings::setImageResolution(const QString& resolution) {
  if (resolution != imageResolution()) {
    m_settings->setValue("image/resolution", resolution);
    emit imageResolutionChanged();
  }
}

QString Settings::videoAspectRatio() const {
  // This is not used for anything so we will return an empty string for now
  // which will make the backend return all resolutions.

  return QString();
}

void Settings::setVideoAspectRatio(const QString& aspectRatio) {
  Q_UNUSED(aspectRatio);

  // This is not used for anything so we will just ignore it.
}

QString Settings::videoResolution() const {
  return m_settings->value("video/resolution", DEFAULT_VIDEO_RESOLUTION).toString();
}

void Settings::setVideoResolution(const QString& resolution) {
  if (resolution != videoResolution()) {
    m_settings->setValue("video/resolution", resolution);
    emit videoResolutionChanged();
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

bool Settings::isVideoTorchOn() const {
  return m_settings->value("video/torchOn", DEFAULT_VIDEO_TORCH_ON).toBool();
}

void Settings::setVideoTorchOn(bool on) {
  if (isVideoTorchOn() != on) {
    m_settings->setValue("video/torchOn", on);
    emit videoTorchOnChanged();
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

bool Settings::isZoomAsShutterEnabled() {
  return m_settings->value("camera/zoomAsShutter", DEFAULT_ZOOM_AS_SHUTTER).toBool();
}

void Settings::setZoomAsShutterEnabled(bool enabled) {
  if (isZoomAsShutterEnabled() != enabled) {
    m_settings->setValue("camera/zoomAsShutter", enabled);

    emit zoomAsShutterChanged();
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
