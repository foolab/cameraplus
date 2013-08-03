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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class QSettings;

class Settings : public QObject {
  Q_OBJECT

  Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged);
  Q_PROPERTY(QString creatorName READ creatorName WRITE setCreatorName NOTIFY creatorNameChanged);
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

  Q_PROPERTY(QString primaryImageAspectRatio READ primaryImageAspectRatio WRITE setPrimaryImageAspectRatio NOTIFY primaryImageAspectRatioChanged);
  Q_PROPERTY(QString primaryImageResolution READ primaryImageResolution WRITE setPrimaryImageResolution NOTIFY primaryImageResolutionChanged);
  Q_PROPERTY(QString primaryVideoAspectRatio READ primaryVideoAspectRatio WRITE setPrimaryVideoAspectRatio NOTIFY primaryVideoAspectRatioChanged);
  Q_PROPERTY(QString primaryVideoResolution READ primaryVideoResolution WRITE setPrimaryVideoResolution NOTIFY primaryVideoResolutionChanged);
  Q_PROPERTY(QString secondaryImageAspectRatio READ secondaryImageAspectRatio WRITE setSecondaryImageAspectRatio NOTIFY secondaryImageAspectRatioChanged);
  Q_PROPERTY(QString secondaryImageResolution READ secondaryImageResolution WRITE setSecondaryImageResolution NOTIFY secondaryImageResolutionChanged);
  Q_PROPERTY(QString secondaryVideoAspectRatio READ secondaryVideoAspectRatio WRITE setSecondaryVideoAspectRatio NOTIFY secondaryVideoAspectRatioChanged);
  Q_PROPERTY(QString secondaryVideoResolution READ secondaryVideoResolution WRITE setSecondaryVideoResolution NOTIFY secondaryVideoResolutionChanged);

  Q_PROPERTY(bool soundEnabled READ isSoundEnabled WRITE setSoundEnabled NOTIFY soundEnabledChanged);
  Q_PROPERTY(bool videoTorchOn READ isVideoTorchOn WRITE setVideoTorchOn NOTIFY videoTorchOnChanged);

  Q_PROPERTY(bool showToolBar READ isToolBarShown WRITE setToolBarShown NOTIFY toolBarShownChanged);
  Q_PROPERTY(bool videoMuted READ isVideoMuted WRITE setVideoMuted NOTIFY videoMutedChanged);

  Q_PROPERTY(bool gridEnabled READ isGridEnabled WRITE setGridEnabled NOTIFY gridEnabledChanged);

  Q_PROPERTY(bool faceDetectionEnabled READ isFaceDetectionEnabled WRITE setFaceDetectionEnabled NOTIFY faceDetectionEnabledChanged);
  Q_PROPERTY(bool zoomAsShutter READ isZoomAsShutterEnabled WRITE setZoomAsShutterEnabled NOTIFY zoomAsShutterChanged);
  Q_PROPERTY(int device READ device WRITE setDevice NOTIFY deviceChanged);

public:
  Settings(QObject *parent = 0);
  ~Settings();

  int mode() const;
  void setMode(int mode);

  QString creatorName() const;
  void setCreatorName(const QString& name);

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

  bool isSoundEnabled() const;
  void setSoundEnabled(bool enabled);

  bool isVideoTorchOn() const;
  void setVideoTorchOn(bool on);

  bool isToolBarShown() const;
  void setToolBarShown(bool shown);

  bool isVideoMuted() const;
  void setVideoMuted(bool muted);

  bool isGridEnabled() const;
  void setGridEnabled(bool enabled);

  bool isFaceDetectionEnabled() const;
  void setFaceDetectionEnabled(bool enabled);

  bool isZoomAsShutterEnabled();
  void setZoomAsShutterEnabled(bool enabled);

  int device() const;
  void setDevice(int device);

  QString primaryImageAspectRatio() const;
  void setPrimaryImageAspectRatio(const QString& aspectRatio);

  QString primaryImageResolution() const;
  void setPrimaryImageResolution(const QString& resolution);

  QString primaryVideoAspectRatio() const;
  void setPrimaryVideoAspectRatio(const QString& aspectRatio);

  QString primaryVideoResolution() const;
  void setPrimaryVideoResolution(const QString& resolution);

  QString secondaryImageAspectRatio() const;
  void setSecondaryImageAspectRatio(const QString& aspectRatio);

  QString secondaryImageResolution() const;
  void setSecondaryImageResolution(const QString& resolution);

  QString secondaryVideoAspectRatio() const;
  void setSecondaryVideoAspectRatio(const QString& aspectRatio);

  QString secondaryVideoResolution() const;
  void setSecondaryVideoResolution(const QString& resolution);

signals:
  void modeChanged();
  void creatorNameChanged();
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
  void soundEnabledChanged();
  void videoTorchOnChanged();
  void toolBarShownChanged();
  void videoMutedChanged();
  void gridEnabledChanged();
  void faceDetectionEnabledChanged();
  void zoomAsShutterChanged();
  void deviceChanged();
  void primaryImageAspectRatioChanged();
  void primaryImageResolutionChanged();
  void primaryVideoAspectRatioChanged();
  void primaryVideoResolutionChanged();
  void secondaryImageAspectRatioChanged();
  void secondaryImageResolutionChanged();
  void secondaryVideoAspectRatioChanged();
  void secondaryVideoResolutionChanged();

private:
  QSettings *m_settings;
};

#endif /* SETTINGS_H */
