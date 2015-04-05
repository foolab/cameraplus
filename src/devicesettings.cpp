// -*- c++ -*-

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

#include "devicesettings.h"
#include "settings.h"
#include <QSettings>

#define IMAGE_RESOLUTION_KEY        "imageResolution"
#define VIDEO_RESOLUTION_KEY        "videoResolution"
#define FAST_CAPTURE_RESOLUTION_KEY "imageFastCaptureResolution"
#define IMAGE_SCENE_MODE_KEY        "imageSceneMode"
#define VIDEO_SCENE_MODE_KEY        "videoSceneMode"
#define IMAGE_COLOR_FILTER_KEY      "imageColorFilter"
#define VIDEO_COLOR_FILTER_KEY      "videoColorFilter"
#define IMAGE_WHITE_BALANCE_KEY     "imageWhiteBalance"
#define VIDEO_WHITE_BALANCE_KEY     "videoWhiteBalance"
#define IMAGE_EV_COMP_KEY           "imageEvComp"
#define VIDEO_EV_COMP_KEY           "videoEvComp"
#define IMAGE_FLASH_MODE_KEY        "imageFlashMode"
#define VIDEO_TORCH_ON_KEY          "videoTorchOn"
#define IMAGE_ISO_KEY               "imageIso"
#define VIDEO_MUTE_KEY              "videoMute"

PrimaryDeviceSettings::PrimaryDeviceSettings(QObject *parent) :
  DeviceSettings(parent) {

  QHash<QString, QVariant> props;

  props[FAST_CAPTURE_RESOLUTION_KEY] = "image-medium-16:9";
  props[IMAGE_SCENE_MODE_KEY] = 6;
  props[VIDEO_SCENE_MODE_KEY] = 6;
  props[IMAGE_COLOR_FILTER_KEY] = 0;
  props[VIDEO_COLOR_FILTER_KEY] = 0;
  props[IMAGE_WHITE_BALANCE_KEY] = 0;
  props[VIDEO_WHITE_BALANCE_KEY] = 0;
  props[IMAGE_EV_COMP_KEY] = 0.0;
  props[VIDEO_EV_COMP_KEY] = 0.0;
  props[IMAGE_FLASH_MODE_KEY] = 0;
  props[VIDEO_TORCH_ON_KEY] = false;
  props[IMAGE_ISO_KEY] = 0;
  props[VIDEO_MUTE_KEY] = false;

  init("primary", props);
}

SecondaryDeviceSettings::SecondaryDeviceSettings(QObject *parent) :
  DeviceSettings(parent) {

  QHash<QString, QVariant> props;

  props[FAST_CAPTURE_RESOLUTION_KEY] = "";
  props[IMAGE_SCENE_MODE_KEY] = 6;
  props[VIDEO_SCENE_MODE_KEY] = 6;
  props[IMAGE_COLOR_FILTER_KEY] = 0;
  props[VIDEO_COLOR_FILTER_KEY] = 0;
  props[IMAGE_WHITE_BALANCE_KEY] = 0;
  props[VIDEO_WHITE_BALANCE_KEY] = 0;
  props[IMAGE_EV_COMP_KEY] = 0.0;
  props[VIDEO_EV_COMP_KEY] = 0.0;
  props[IMAGE_FLASH_MODE_KEY] = 0;
  props[VIDEO_TORCH_ON_KEY] = false;
  props[IMAGE_ISO_KEY] = 0;
  props[VIDEO_MUTE_KEY] = false;

  init("secondary", props);
}

DeviceSettings::DeviceSettings(QObject *parent) :
  QObject(parent),
  m_settings(0) {

}

DeviceSettings::~DeviceSettings() {
  m_settings = 0;
}

void DeviceSettings::init(const QString& id, const QHash<QString, QVariant>& props) {
  m_id = id;
  m_props = props;
}

Settings *DeviceSettings::settings() const {
  return m_settings;
}

void DeviceSettings::setSettings(Settings *settings) {
  if (m_settings != settings) {
    m_settings = settings;

    emit settingsChanged();
  }
}

QVariant DeviceSettings::get(const QString& settingsKey, const QString& hashKey) const {
  QVariant defaultValue;

  if (!m_props.contains(hashKey)) {
    // Try to read it from deviceConfig
    QSettings s(m_config, QSettings::IniFormat);
    s.beginGroup(m_id);
    defaultValue = s.value(hashKey);
    s.endGroup();

    // Insert it in case we need to get it again
    if (defaultValue.isValid()) {
      m_props.insert(hashKey, defaultValue);
    }
  } else {
    defaultValue = m_props[hashKey];
  }

  if (!defaultValue.isValid()) {
    qFatal("no default value for %s/%s", qPrintable(m_id), qPrintable(hashKey));

    return QVariant();
  }

  QString key = QString("%1/%2").arg(m_id).arg(settingsKey);

  return m_settings->value(key, defaultValue);
}

void DeviceSettings::set(const QString& key, const QVariant& value) {
  QString settingsKey = QString("%1/%2").arg(m_id).arg(key);

  m_settings->setValue(settingsKey, value);
}

int DeviceSettings::imageSceneMode() const {
  return get("image/sceneMode", IMAGE_SCENE_MODE_KEY).toInt();
}

void DeviceSettings::setImageSceneMode(int mode) {
  if (imageSceneMode() != mode) {
    set("image/sceneMode", mode);
    emit imageSceneModeChanged();
  }
}

int DeviceSettings::imageColorFilter() const {
  return get("image/colorFilter", IMAGE_COLOR_FILTER_KEY).toInt();
}

void DeviceSettings::setImageColorFilter(int filter) {
  if (imageColorFilter() != filter) {
    set("image/colorFilter", filter);
    emit imageColorFilterChanged();
  }
}

int DeviceSettings::imageWhiteBalance() const {
  return get("image/whiteBalance", IMAGE_WHITE_BALANCE_KEY).toInt();
}

void DeviceSettings::setImageWhiteBalance(int wb) {
  if (imageWhiteBalance() != wb) {
    set("image/whiteBalance", wb);
    emit imageWhiteBalanceChanged();
  }
}

qreal DeviceSettings::imageEvComp() const {
  return get("image/evComp", IMAGE_EV_COMP_KEY).toReal();
}

void DeviceSettings::setImageEvComp(qreal ev) {
  if (!qFuzzyCompare(imageEvComp(), ev)) {
    set("image/evComp", ev);
    emit imageEvCompChanged();
  }
}

int DeviceSettings::videoSceneMode() const {
  return get("video/sceneMode", VIDEO_SCENE_MODE_KEY).toInt();
}

void DeviceSettings::setVideoSceneMode(int mode) {
  if (videoSceneMode() != mode) {
    set("video/sceneMode", mode);
    emit videoSceneModeChanged();
  }
}

int DeviceSettings::videoColorFilter() const {
  return get("video/colorFilter", VIDEO_COLOR_FILTER_KEY).toInt();
}

void DeviceSettings::setVideoColorFilter(int filter) {
  if (videoColorFilter() != filter) {
    set("video/colorFilter", filter);
    emit videoColorFilterChanged();
  }
}

int DeviceSettings::videoWhiteBalance() const {
  return get("video/whiteBalance", VIDEO_WHITE_BALANCE_KEY).toInt();
}

void DeviceSettings::setVideoWhiteBalance(int wb) {
  if (videoWhiteBalance() != wb) {
    set("video/whiteBalance", wb);
    emit videoWhiteBalanceChanged();
  }
}

qreal DeviceSettings::videoEvComp() const {
  return get("video/evComp", VIDEO_EV_COMP_KEY).toReal();
}

void DeviceSettings::setVideoEvComp(qreal ev) {
  if (!qFuzzyCompare(videoEvComp(), ev)) {
    set("video/evComp", ev);
    emit videoEvCompChanged();
  }
}

int DeviceSettings::imageFlashMode() const {
  return get("image/flashMode", IMAGE_FLASH_MODE_KEY).toInt();
}

void DeviceSettings::setImageFlashMode(int mode) {
  if (imageFlashMode() != mode) {
    set("image/flashMode", mode);
    emit imageFlashModeChanged();
  }
}

int DeviceSettings::imageIso() const {
  return get("image/iso", IMAGE_ISO_KEY).toInt();
}

void DeviceSettings::setImageIso(int iso) {
  if (imageIso() != iso) {
    set("image/iso", iso);
    emit imageIsoChanged();
  }
}

QString DeviceSettings::imageResolution() const {
  return get("image/resolution", IMAGE_RESOLUTION_KEY).toString();
}

void DeviceSettings::setImageResolution(const QString& resolution) {
  if (imageResolution() != resolution) {
    set("image/resolution", resolution);
    emit imageResolutionChanged();
  }
}

QString DeviceSettings::videoResolution() const {
  return get("video/resolution", VIDEO_RESOLUTION_KEY).toString();
}

void DeviceSettings::setVideoResolution(const QString& resolution) {
  if (videoResolution() != resolution) {
    set("video/resolution", resolution);
    emit videoResolutionChanged();
  }
}

bool DeviceSettings::isVideoTorchOn() const {
  return get("video/torchOn", VIDEO_TORCH_ON_KEY).toBool();
}

void DeviceSettings::setVideoTorchOn(bool on) {
  if (isVideoTorchOn() != on) {
    set("video/torchOn", on);
    emit videoTorchOnChanged();
  }
}

bool DeviceSettings::isVideoMuted() const {
  return get("video/mute", VIDEO_MUTE_KEY).toBool();
}

void DeviceSettings::setVideoMuted(bool muted) {
  if (isVideoMuted() != muted) {
    set("video/mute", muted);
    emit videoMutedChanged();
  }
}

QString DeviceSettings::fastCaptureResolution() const {
  return get("image/fastCaptureResolution", FAST_CAPTURE_RESOLUTION_KEY).toString();
}

void DeviceSettings::setFastCaptureResolution(const QString& resolution) {
  if (fastCaptureResolution() != resolution) {
    set("image/fastCaptureResolution", resolution);
    emit fastCaptureResolutionChanged();
  }
}

QString DeviceSettings::deviceConfig() const {
  return m_config;
}

void DeviceSettings::setDeviceConfig(const QString& conf) {
  if (m_config != conf) {
    m_config = conf;
    emit deviceConfigChanged();
  }
}
