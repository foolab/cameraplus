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

#ifndef DEVICE_SETTINGS_H
#define DEVICE_SETTINGS_H

#include <QObject>
#include <QHash>
#include <QVariant>

class Settings;

class DeviceSettings : public QObject {
  Q_OBJECT
  Q_PROPERTY(Settings *settings READ settings WRITE setSettings NOTIFY settingsChanged);
  Q_PROPERTY(int imageSceneMode READ imageSceneMode WRITE setImageSceneMode NOTIFY imageSceneModeChanged);
  Q_PROPERTY(int imageColorFilter READ imageColorFilter WRITE setImageColorFilter NOTIFY imageColorFilterChanged);
  Q_PROPERTY(int imageWhiteBalance READ imageWhiteBalance WRITE setImageWhiteBalance NOTIFY imageWhiteBalanceChanged);
  Q_PROPERTY(qreal imageEvComp READ imageEvComp WRITE setImageEvComp NOTIFY imageEvCompChanged);
  Q_PROPERTY(int videoSceneMode READ videoSceneMode WRITE setVideoSceneMode NOTIFY videoSceneModeChanged);
  Q_PROPERTY(int videoColorFilter READ videoColorFilter WRITE setVideoColorFilter NOTIFY videoColorFilterChanged);
  Q_PROPERTY(int videoWhiteBalance READ videoWhiteBalance WRITE setVideoWhiteBalance NOTIFY videoWhiteBalanceChanged);
  Q_PROPERTY(qreal videoEvComp READ videoEvComp WRITE setVideoEvComp NOTIFY videoEvCompChanged);
  Q_PROPERTY(int imageFlashMode READ imageFlashMode WRITE setImageFlashMode NOTIFY imageFlashModeChanged);
  Q_PROPERTY(int imageIso READ imageIso WRITE setImageIso NOTIFY imageIsoChanged);
  Q_PROPERTY(QString imageAspectRatio READ imageAspectRatio WRITE setImageAspectRatio NOTIFY imageAspectRatioChanged);
  Q_PROPERTY(QString imageResolution READ imageResolution WRITE setImageResolution NOTIFY imageResolutionChanged);
  Q_PROPERTY(QString videoAspectRatio READ videoAspectRatio WRITE setVideoAspectRatio NOTIFY videoAspectRatioChanged);
  Q_PROPERTY(QString videoResolution READ videoResolution WRITE setVideoResolution NOTIFY videoResolutionChanged);
  Q_PROPERTY(bool videoTorchOn READ isVideoTorchOn WRITE setVideoTorchOn NOTIFY videoTorchOnChanged);
  Q_PROPERTY(bool videoMuted READ isVideoMuted WRITE setVideoMuted NOTIFY videoMutedChanged);

public:
  DeviceSettings(QObject *parent = 0);
  virtual ~DeviceSettings();

  Settings *settings() const;
  void setSettings(Settings *settings);

  int imageSceneMode() const;
  void setImageSceneMode(int mode);

  int imageColorFilter() const;
  void setImageColorFilter(int filter);

  int imageWhiteBalance() const;
  void setImageWhiteBalance(int wb);

  qreal imageEvComp() const;
  void setImageEvComp(qreal ev);

  int videoSceneMode() const;
  void setVideoSceneMode(int mode);

  int videoColorFilter() const;
  void setVideoColorFilter(int filter);

  int videoWhiteBalance() const;
  void setVideoWhiteBalance(int wb);

  qreal videoEvComp() const;
  void setVideoEvComp(qreal ev);

  int imageFlashMode() const;
  void setImageFlashMode(int mode);

  int imageIso() const;
  void setImageIso(int iso);

  QString imageAspectRatio() const;
  void setImageAspectRatio(const QString& aspectRatio);

  QString imageResolution() const;
  void setImageResolution(const QString& resolution);

  QString videoAspectRatio() const;
  void setVideoAspectRatio(const QString& aspectRatio);

  QString videoResolution() const;
  void setVideoResolution(const QString& resolution);

  bool isVideoTorchOn() const;
  void setVideoTorchOn(bool on);

  bool isVideoMuted() const;
  void setVideoMuted(bool muted);

signals:
  void settingsChanged();
  void imageSceneModeChanged();
  void imageColorFilterChanged();
  void imageWhiteBalanceChanged();
  void imageEvCompChanged();
  void videoSceneModeChanged();
  void videoColorFilterChanged();
  void videoWhiteBalanceChanged();
  void videoEvCompChanged();
  void imageFlashModeChanged();
  void imageIsoChanged();
  void imageAspectRatioChanged();
  void imageResolutionChanged();
  void videoAspectRatioChanged();
  void videoResolutionChanged();
  void videoTorchOnChanged();
  void videoMutedChanged();

protected:
  void init(const QString& id, const QHash<QString, QVariant>& props);

private:
  QVariant get(const QString& settingsKey, const QString& hashKey) const;
  void set(const QString& key, const QVariant& value);

  QString m_id;
  Settings *m_settings;
  QHash<QString, QVariant> m_props;
};

class PrimaryDeviceSettings : public DeviceSettings {
  Q_OBJECT

public:
  PrimaryDeviceSettings(QObject *parent = 0);
};

class SecondaryDeviceSettings : public DeviceSettings {
  Q_OBJECT

public:
  SecondaryDeviceSettings(QObject *parent = 0);
};

#endif /* DEVICE_SETTINGS_H */
