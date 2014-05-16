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

#ifndef DEVICE_FEATURES_H
#define DEVICE_FEATURES_H

#include <QObject>

class QSettings;

class DeviceFeatures : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool isAutoFlashModeSupported READ isAutoFlashModeSupported CONSTANT);
  Q_PROPERTY(bool isOnFlashModeSupported READ isOnFlashModeSupported CONSTANT);
  Q_PROPERTY(bool isOffFlashModeSupported READ isOffFlashModeSupported CONSTANT);
  Q_PROPERTY(bool isRedEyeFlashModeSupported READ isRedEyeFlashModeSupported CONSTANT);
  Q_PROPERTY(int numberOfFlashModes READ numberOfFlashModes CONSTANT);
  Q_PROPERTY(bool isCloseupImageSceneModeSupported READ isCloseupImageSceneModeSupported CONSTANT);
  Q_PROPERTY(bool isPortraitImageSceneModeSupported READ isPortraitImageSceneModeSupported CONSTANT);
  Q_PROPERTY(bool isLandscapeImageSceneModeSupported READ isLandscapeImageSceneModeSupported CONSTANT);
  Q_PROPERTY(bool isSportImageSceneModeSupported READ isSportImageSceneModeSupported CONSTANT);
  Q_PROPERTY(bool isNightImageSceneModeSupported READ isNightImageSceneModeSupported CONSTANT);
  Q_PROPERTY(bool isAutoImageSceneModeSupported READ isAutoImageSceneModeSupported CONSTANT);
  Q_PROPERTY(int numberOfImageSceneModes READ numberOfImageSceneModes CONSTANT);
  Q_PROPERTY(bool isCloseupVideoSceneModeSupported READ isCloseupVideoSceneModeSupported CONSTANT);
  Q_PROPERTY(bool isPortraitVideoSceneModeSupported READ isPortraitVideoSceneModeSupported CONSTANT);
  Q_PROPERTY(bool isLandscapeVideoSceneModeSupported READ isLandscapeVideoSceneModeSupported CONSTANT);
  Q_PROPERTY(bool isSportVideoSceneModeSupported READ isSportVideoSceneModeSupported CONSTANT);
  Q_PROPERTY(bool isNightVideoSceneModeSupported READ isNightVideoSceneModeSupported CONSTANT);
  Q_PROPERTY(bool isAutoVideoSceneModeSupported READ isAutoVideoSceneModeSupported CONSTANT);
  Q_PROPERTY(int numberOfVideoSceneModes READ numberOfVideoSceneModes CONSTANT);
  Q_PROPERTY(bool isNormalImageColorToneSupported READ isNormalImageColorToneSupported CONSTANT);
  Q_PROPERTY(bool isSepiaImageColorToneSupported READ isSepiaImageColorToneSupported CONSTANT);
  Q_PROPERTY(bool isNegativeImageColorToneSupported READ isNegativeImageColorToneSupported CONSTANT);
  Q_PROPERTY(bool isGrayScaleImageColorToneSupported READ isGrayScaleImageColorToneSupported CONSTANT);
  Q_PROPERTY(bool isVividImageColorToneSupported READ isVividImageColorToneSupported CONSTANT);
  Q_PROPERTY(bool isSolarizeImageColorToneSupported READ isSolarizeImageColorToneSupported CONSTANT);
  Q_PROPERTY(int numberOfImageColorTones READ numberOfImageColorTones CONSTANT);
  Q_PROPERTY(bool isNormalVideoColorToneSupported READ isNormalVideoColorToneSupported CONSTANT);
  Q_PROPERTY(bool isSepiaVideoColorToneSupported READ isSepiaVideoColorToneSupported CONSTANT);
  Q_PROPERTY(bool isNegativeVideoColorToneSupported READ isNegativeVideoColorToneSupported CONSTANT);
  Q_PROPERTY(bool isGrayScaleVideoColorToneSupported READ isGrayScaleVideoColorToneSupported CONSTANT);
  Q_PROPERTY(bool isVividVideoColorToneSupported READ isVividVideoColorToneSupported CONSTANT);
  Q_PROPERTY(bool isSolarizeVideoColorToneSupported READ isSolarizeVideoColorToneSupported CONSTANT);
  Q_PROPERTY(int numberOfVideoColorTones READ numberOfVideoColorTones CONSTANT);
  Q_PROPERTY(bool isAutoImageWhiteBalanceModeSupported READ isAutoImageWhiteBalanceModeSupported CONSTANT);
  Q_PROPERTY(bool isSunnyImageWhiteBalanceModeSupported READ isSunnyImageWhiteBalanceModeSupported CONSTANT);
  Q_PROPERTY(bool isCloudyImageWhiteBalanceModeSupported READ isCloudyImageWhiteBalanceModeSupported CONSTANT);
  Q_PROPERTY(bool isFlourescentImageWhiteBalanceModeSupported READ isFlourescentImageWhiteBalanceModeSupported CONSTANT);
  Q_PROPERTY(bool isTungstenImageWhiteBalanceModeSupported READ isTungstenImageWhiteBalanceModeSupported CONSTANT);
  Q_PROPERTY(int numberOfImageWhiteBalanceModes READ numberOfImageWhiteBalanceModes CONSTANT);
  Q_PROPERTY(bool isAutoVideoWhiteBalanceModeSupported READ isAutoVideoWhiteBalanceModeSupported CONSTANT);
  Q_PROPERTY(bool isSunnyVideoWhiteBalanceModeSupported READ isSunnyVideoWhiteBalanceModeSupported CONSTANT);
  Q_PROPERTY(bool isCloudyVideoWhiteBalanceModeSupported READ isCloudyVideoWhiteBalanceModeSupported CONSTANT);
  Q_PROPERTY(bool isFlourescentVideoWhiteBalanceModeSupported READ isFlourescentVideoWhiteBalanceModeSupported CONSTANT);
  Q_PROPERTY(bool isTungstenVideoWhiteBalanceModeSupported READ isTungstenVideoWhiteBalanceModeSupported CONSTANT);
  Q_PROPERTY(int numberOfVideoWhiteBalanceModes READ numberOfVideoWhiteBalanceModes CONSTANT);
  Q_PROPERTY(bool is0IsoModeSupported READ is0IsoModeSupported CONSTANT);
  Q_PROPERTY(bool is100IsoModeSupported READ is100IsoModeSupported CONSTANT);
  Q_PROPERTY(bool is200IsoModeSupported READ is200IsoModeSupported CONSTANT);
  Q_PROPERTY(bool is400IsoModeSupported READ is400IsoModeSupported CONSTANT);
  Q_PROPERTY(bool is800IsoModeSupported READ is800IsoModeSupported CONSTANT);
  Q_PROPERTY(int numberOfIsoModes READ numberOfIsoModes CONSTANT);
  Q_PROPERTY(bool isVideoTorchSupported READ isVideoTorchSupported CONSTANT);
  Q_PROPERTY(bool isTouchFocusSupported READ isTouchFocusSupported CONSTANT);
  Q_PROPERTY(bool isFaceDetectionSupported READ isFaceDetectionSupported CONSTANT);
  Q_PROPERTY(bool isAutoFocusSupported READ isAutoFocusSupported CONSTANT);

public:
  DeviceFeatures(const QString& id, QObject *parent = 0);
  virtual ~DeviceFeatures();

  bool isAutoFlashModeSupported() const;
  bool isOnFlashModeSupported() const;
  bool isOffFlashModeSupported() const;
  bool isRedEyeFlashModeSupported() const;
  int numberOfFlashModes() const;
  bool isCloseupImageSceneModeSupported() const;
  bool isPortraitImageSceneModeSupported() const;
  bool isLandscapeImageSceneModeSupported() const;
  bool isSportImageSceneModeSupported() const;
  bool isNightImageSceneModeSupported() const;
  bool isAutoImageSceneModeSupported() const;
  int numberOfImageSceneModes() const;
  bool isCloseupVideoSceneModeSupported() const;
  bool isPortraitVideoSceneModeSupported() const;
  bool isLandscapeVideoSceneModeSupported() const;
  bool isSportVideoSceneModeSupported() const;
  bool isNightVideoSceneModeSupported() const;
  bool isAutoVideoSceneModeSupported() const;
  int numberOfVideoSceneModes() const;
  bool isNormalImageColorToneSupported() const;
  bool isSepiaImageColorToneSupported() const;
  bool isNegativeImageColorToneSupported() const;
  bool isGrayScaleImageColorToneSupported() const;
  bool isVividImageColorToneSupported() const;
  bool isSolarizeImageColorToneSupported() const;
  int numberOfImageColorTones() const;
  bool isNormalVideoColorToneSupported() const;
  bool isSepiaVideoColorToneSupported() const;
  bool isNegativeVideoColorToneSupported() const;
  bool isGrayScaleVideoColorToneSupported() const;
  bool isVividVideoColorToneSupported() const;
  bool isSolarizeVideoColorToneSupported() const;
  int numberOfVideoColorTones() const;
  bool isAutoImageWhiteBalanceModeSupported() const;
  bool isSunnyImageWhiteBalanceModeSupported() const;
  bool isCloudyImageWhiteBalanceModeSupported() const;
  bool isFlourescentImageWhiteBalanceModeSupported() const;
  bool isTungstenImageWhiteBalanceModeSupported() const;
  int numberOfImageWhiteBalanceModes() const;
  bool isAutoVideoWhiteBalanceModeSupported() const;
  bool isSunnyVideoWhiteBalanceModeSupported() const;
  bool isCloudyVideoWhiteBalanceModeSupported() const;
  bool isFlourescentVideoWhiteBalanceModeSupported() const;
  bool isTungstenVideoWhiteBalanceModeSupported() const;
  int numberOfVideoWhiteBalanceModes() const;
  bool is0IsoModeSupported() const;
  bool is100IsoModeSupported() const;
  bool is200IsoModeSupported() const;
  bool is400IsoModeSupported() const;
  bool is800IsoModeSupported() const;
  int numberOfIsoModes() const;
  bool isVideoTorchSupported() const;
  bool isTouchFocusSupported() const;
  bool isFaceDetectionSupported() const;
  bool isAutoFocusSupported() const;

private:
  const QString m_id;
  QSettings *m_settings;
};

class PrimaryDeviceFeatures : public DeviceFeatures {
  Q_OBJECT

public:
  PrimaryDeviceFeatures(QObject *parent = 0);
};

class SecondaryDeviceFeatures : public DeviceFeatures {
  Q_OBJECT

public:
  SecondaryDeviceFeatures(QObject *parent = 0);
};

#endif /* DEVICE_FEATURES_H */
