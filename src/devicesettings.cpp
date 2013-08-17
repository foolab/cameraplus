// -*- c++ -*-

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

#include "devicesettings.h"
#include <QDebug>

#define IMAGE_RESOLUTION_KEY        "imageResolution"
#define IMAGE_ASPECT_RATIO_KEY      "imageAspectRatio"
#define VIDEO_RESOLUTION_KEY        "videoResolution"
#define VIDEO_ASPECT_RATIO_KEY      "videoAspectRatio"
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

DeviceSettings::DeviceSettings(const QHash<QString, QVariant>& defaultSettings) :
  m_settings(defaultSettings) {

}

DeviceSettings::~DeviceSettings() {

}

QVariant DeviceSettings::value(const QString& key) const {
  if (!m_settings.contains(key)) {
    qFatal("%s not found", qPrintable(key));

    return QVariant();
  }

  return m_settings[key];
}

QVariant DeviceSettings::defaultImageResolution() const {
  return value(IMAGE_RESOLUTION_KEY);
}

QVariant DeviceSettings::defaultVideoResolution() const {
  return value(VIDEO_RESOLUTION_KEY);
}

QVariant DeviceSettings::defaultImageAspectRatio() const {
  return value(IMAGE_ASPECT_RATIO_KEY);
}

QVariant DeviceSettings::defaultVideoAspectRatio() const {
  return value(VIDEO_ASPECT_RATIO_KEY);
}

QVariant DeviceSettings::defaultImageSceneMode() const {
  return value(IMAGE_SCENE_MODE_KEY);
}

QVariant DeviceSettings::defaultVideoSceneMode() const {
  return value(VIDEO_SCENE_MODE_KEY);
}

QVariant DeviceSettings::defaultImageColorFilter() const {
  return value(IMAGE_COLOR_FILTER_KEY);
}

QVariant DeviceSettings::defaultVideoColorFilter() const {
  return value(VIDEO_COLOR_FILTER_KEY);
}

QVariant DeviceSettings::defaultImageWhiteBalance() const {
  return value(IMAGE_WHITE_BALANCE_KEY);
}

QVariant DeviceSettings::defaultVideoWhiteBalance() const {
  return value(VIDEO_WHITE_BALANCE_KEY);
}

QVariant DeviceSettings::defaultImageEvComp() const {
  return value(IMAGE_EV_COMP_KEY);
}

QVariant DeviceSettings::defaultVideoEvComp() const {
  return value(VIDEO_EV_COMP_KEY);
}

QVariant DeviceSettings::defaultImageFlashMode() const {
  return value(IMAGE_FLASH_MODE_KEY);
}

QVariant DeviceSettings::defaultImageIso() const {
  return value(IMAGE_ISO_KEY);
}

QVariant DeviceSettings::defaultVideoTorchOn() const {
  return value(VIDEO_TORCH_ON_KEY);
}

PrimaryDeviceSettings::PrimaryDeviceSettings() :
  DeviceSettings(settings()) {

}

QString PrimaryDeviceSettings::id() const {
  return "primary";
}

QHash<QString, QVariant> PrimaryDeviceSettings::settings() const {
  QHash<QString, QVariant> hash;

  hash[IMAGE_RESOLUTION_KEY] = "high";
  hash[IMAGE_ASPECT_RATIO_KEY] = "16:9";
  hash[VIDEO_RESOLUTION_KEY] = "high";
  hash[VIDEO_ASPECT_RATIO_KEY] = "16:9";
  hash[IMAGE_SCENE_MODE_KEY] = 6;
  hash[VIDEO_SCENE_MODE_KEY] = 6;
  hash[IMAGE_COLOR_FILTER_KEY] = 0;
  hash[VIDEO_COLOR_FILTER_KEY] = 0;
  hash[IMAGE_WHITE_BALANCE_KEY] = 0;
  hash[VIDEO_WHITE_BALANCE_KEY] = 0;
  hash[IMAGE_EV_COMP_KEY] = 0.0;
  hash[VIDEO_EV_COMP_KEY] = 0.0;
  hash[IMAGE_FLASH_MODE_KEY] = 0;
  hash[VIDEO_TORCH_ON_KEY] = false;
  hash[IMAGE_ISO_KEY] = 0;

  return hash;
}

SecondaryDeviceSettings::SecondaryDeviceSettings() :
  DeviceSettings(settings()) {

}

QString SecondaryDeviceSettings::id() const {
  return "secondary";
}

QHash<QString, QVariant> SecondaryDeviceSettings::settings() const {
  QHash<QString, QVariant> hash;

  hash[IMAGE_RESOLUTION_KEY] = "low";
  hash[IMAGE_ASPECT_RATIO_KEY] = "4:3";
  hash[VIDEO_RESOLUTION_KEY] = "low";
  hash[VIDEO_ASPECT_RATIO_KEY] = "4:3";
  hash[IMAGE_SCENE_MODE_KEY] = 6;
  hash[VIDEO_SCENE_MODE_KEY] = 6;
  hash[IMAGE_COLOR_FILTER_KEY] = 0;
  hash[VIDEO_COLOR_FILTER_KEY] = 0;
  hash[IMAGE_WHITE_BALANCE_KEY] = 0;
  hash[VIDEO_WHITE_BALANCE_KEY] = 0;
  hash[IMAGE_EV_COMP_KEY] = 0.0;
  hash[VIDEO_EV_COMP_KEY] = 0.0;
  hash[IMAGE_FLASH_MODE_KEY] = 0;
  hash[VIDEO_TORCH_ON_KEY] = false;
  hash[IMAGE_ISO_KEY] = 0;

  return hash;
}
