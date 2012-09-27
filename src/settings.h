// -*- c++ -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class QSettings;

class Settings : public QObject {
  Q_OBJECT

  Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged);
  Q_PROPERTY(QString creatorName READ creatorName WRITE setCreatorName NOTIFY creatorNameChanged);
  Q_PROPERTY(int postCaptureTimeout READ postCaptureTimeout WRITE setPostCaptureTimeout NOTIFY postCaptureTimeoutChanged);
  Q_PROPERTY(bool useGps READ useGps WRITE setUseGps NOTIFY useGpsChanged);
  Q_PROPERTY(bool useGeotags READ useGeotags WRITE setUseGeotags NOTIFY useGeotagsChanged);
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

  Q_PROPERTY(bool soundEnabled READ isSoundEnabled WRITE setSoundEnabled NOTIFY soundEnabledChanged);

public:
  Settings(QObject *parent = 0);
  ~Settings();

  int mode() const;
  void setMode(int mode);

  QString creatorName() const;
  void setCreatorName(const QString& name);

  int postCaptureTimeout() const;
  void setPostCaptureTimeout(int timeout);

  bool useGps() const;
  void setUseGps(bool enable);

  bool useGeotags() const;
  void setUseGeotags(bool enable);

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

  bool isSoundEnabled() const;
  void setSoundEnabled(bool enabled);

signals:
  void modeChanged();
  void creatorNameChanged();
  void postCaptureTimeoutChanged();
  void useGpsChanged();
  void useGeotagsChanged();
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
  void soundEnabledChanged();

private:
  QSettings *m_settings;
};

#endif /* SETTINGS_H */
