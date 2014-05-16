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

#include "devicefeatures.h"
#include <QSettings>

#ifdef HARMATTAN
#define PATH "/usr/share/cameraplus/config/features.ini"
#else
#define PATH "/usr/share/harbour-cameraplus/share/cameraplus/config/features.ini"
#endif

DeviceFeatures::DeviceFeatures(const QString& id, QObject *parent) :
  QObject(parent),
  m_id(id),
  m_settings(new QSettings(PATH, QSettings::IniFormat, this)) {

}

DeviceFeatures::~DeviceFeatures() {

}

bool DeviceFeatures::isAutoFlashModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("flash-modes")).toList().contains("auto");
}

bool DeviceFeatures::isOnFlashModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("flash-modes")).toList().contains("on");
}

bool DeviceFeatures::isOffFlashModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("flash-modes")).toList().contains("off");
}

bool DeviceFeatures::isRedEyeFlashModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("flash-modes")).toList().contains("red-eye");
}

int DeviceFeatures::numberOfFlashModes() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("flash-modes")).toList().size();
}

bool DeviceFeatures::isManualImageSceneModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-scene-modes")).toList().contains("manual");
}

bool DeviceFeatures::isCloseupImageSceneModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-scene-modes")).toList().contains("closeup");
}

bool DeviceFeatures::isPortraitImageSceneModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-scene-modes")).toList().contains("portrait");
}

bool DeviceFeatures::isLandscapeImageSceneModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-scene-modes")).toList().contains("landscape");
}

bool DeviceFeatures::isSportImageSceneModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-scene-modes")).toList().contains("sport");
}

bool DeviceFeatures::isNightImageSceneModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-scene-modes")).toList().contains("night");
}

bool DeviceFeatures::isAutoImageSceneModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-scene-modes")).toList().contains("auto");
}

int DeviceFeatures::numberOfImageSceneModes() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-scene-modes")).toList().size();
}

bool DeviceFeatures::isManualVideoSceneModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-scene-modes")).toList().contains("manual");
}

bool DeviceFeatures::isCloseupVideoSceneModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-scene-modes")).toList().contains("closeup");
}

bool DeviceFeatures::isPortraitVideoSceneModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-scene-modes")).toList().contains("portrait");
}

bool DeviceFeatures::isLandscapeVideoSceneModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-scene-modes")).toList().contains("landscape");
}

bool DeviceFeatures::isSportVideoSceneModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-scene-modes")).toList().contains("sport");
}

bool DeviceFeatures::isNightVideoSceneModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-scene-modes")).toList().contains("night");
}

bool DeviceFeatures::isAutoVideoSceneModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-scene-modes")).toList().contains("auto");
}

int DeviceFeatures::numberOfVideoSceneModes() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-scene-modes")).toList().size();
}

bool DeviceFeatures::isNormalImageColorToneSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-color-tones")).toList().contains("normal");
}

bool DeviceFeatures::isSepiaImageColorToneSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-color-tones")).toList().contains("sepia");
}

bool DeviceFeatures::isNegativeImageColorToneSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-color-tones")).toList().contains("negative");
}

bool DeviceFeatures::isGrayScaleImageColorToneSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-color-tones")).toList().contains("gray-scale");
}

bool DeviceFeatures::isVividImageColorToneSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-color-tones")).toList().contains("vivid");
}

bool DeviceFeatures::isSolarizeImageColorToneSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-color-tones")).toList().contains("solarize");
}

int DeviceFeatures::numberOfImageColorTones() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-color-tones")).toList().size();
}

bool DeviceFeatures::isNormalVideoColorToneSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-color-tones")).toList().contains("normal");
}

bool DeviceFeatures::isSepiaVideoColorToneSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-color-tones")).toList().contains("sepia");
}

bool DeviceFeatures::isNegativeVideoColorToneSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-color-tones")).toList().contains("negative");
}

bool DeviceFeatures::isGrayScaleVideoColorToneSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-color-tones")).toList().contains("gray-scale");
}

bool DeviceFeatures::isVividVideoColorToneSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-color-tones")).toList().contains("vivid");
}

bool DeviceFeatures::isSolarizeVideoColorToneSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-color-tones")).toList().contains("solarize");
}

int DeviceFeatures::numberOfVideoColorTones() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-color-tones")).toList().size();
}

bool DeviceFeatures::isAutoImageWhiteBalanceModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-white-balance-modes")).toList().contains("auto");
}

bool DeviceFeatures::isSunnyImageWhiteBalanceModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-white-balance-modes")).toList().contains("sunny");
}

bool DeviceFeatures::isCloudyImageWhiteBalanceModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-white-balance-modes")).toList().contains("cloudy");
}

bool DeviceFeatures::isFlourescentImageWhiteBalanceModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-white-balance-modes")).toList().contains("flourescent");
}

bool DeviceFeatures::isTungstenImageWhiteBalanceModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-white-balance-modes")).toList().contains("tungsten");
}

int DeviceFeatures::numberOfImageWhiteBalanceModes() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("image-white-balance-modes")).toList().size();
}

bool DeviceFeatures::isAutoVideoWhiteBalanceModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-white-balance-modes")).toList().contains("auto");
}

bool DeviceFeatures::isSunnyVideoWhiteBalanceModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-white-balance-modes")).toList().contains("sunny");
}

bool DeviceFeatures::isCloudyVideoWhiteBalanceModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-white-balance-modes")).toList().contains("cloudy");
}

bool DeviceFeatures::isFlourescentVideoWhiteBalanceModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-white-balance-modes")).toList().contains("flourescent");
}

bool DeviceFeatures::isTungstenVideoWhiteBalanceModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-white-balance-modes")).toList().contains("tungsten");
}

int DeviceFeatures::numberOfVideoWhiteBalanceModes() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-white-balance-modes")).toList().size();
}

bool DeviceFeatures::is0IsoModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("iso-modes")).toList().contains("0");
}

bool DeviceFeatures::is100IsoModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("iso-modes")).toList().contains("100");
}

bool DeviceFeatures::is200IsoModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("iso-modes")).toList().contains("200");
}

bool DeviceFeatures::is400IsoModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("iso-modes")).toList().contains("400");
}

bool DeviceFeatures::is800IsoModeSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("iso-modes")).toList().contains("800");
}

int DeviceFeatures::numberOfIsoModes() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("iso-modes")).toList().size();
}

bool DeviceFeatures::isVideoTorchSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("video-torch"), false).toBool();
}

bool DeviceFeatures::isTouchFocusSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("touch-focus"), false).toBool();
}

bool DeviceFeatures::isFaceDetectionSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("face-detection"), false).toBool();
}

bool DeviceFeatures::isAutoFocusSupported() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("auto-focus"), false).toBool();
}

PrimaryDeviceFeatures::PrimaryDeviceFeatures(QObject *parent) :
  DeviceFeatures("device-0", parent) {

}

SecondaryDeviceFeatures::SecondaryDeviceFeatures(QObject *parent) :
  DeviceFeatures("device-1", parent) {

}
