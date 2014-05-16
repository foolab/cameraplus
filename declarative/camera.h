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

#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QVariant>
#include <QPointer>

class QtCamera;
class QtCamDevice;
class Sounds;
class NotificationsContainer;
class Zoom;
class Flash;
class Scene;
class EvComp;
class WhiteBalance;
class ColorTone;
class Iso;
class Exposure;
class Aperture;
class NoiseReduction;
class FlickerReduction;
class Focus;
class AutoFocus;
class Roi;
class VideoMute;
class VideoTorch;
class CameraConfig;

class Camera : public QObject {
  Q_OBJECT

  Q_PROPERTY(int deviceCount READ deviceCount NOTIFY deviceCountChanged)
  Q_PROPERTY(QVariant deviceId READ deviceId NOTIFY deviceIdChanged);
  Q_PROPERTY(CameraMode mode READ mode NOTIFY modeChanged);
  Q_PROPERTY(bool idle READ isIdle NOTIFY idleStateChanged);
  Q_PROPERTY(bool running READ isRunning NOTIFY runningStateChanged);
  Q_PROPERTY(Sounds *sounds READ sounds WRITE setSounds NOTIFY soundsChanged);

  Q_PROPERTY(Zoom *zoom READ zoom NOTIFY zoomChanged);
  Q_PROPERTY(Flash *flash READ flash NOTIFY flashChanged);
  Q_PROPERTY(Scene *scene READ scene NOTIFY sceneChanged);
  Q_PROPERTY(EvComp *evComp READ evComp NOTIFY evCompChanged);
  Q_PROPERTY(WhiteBalance *whiteBalance READ whiteBalance NOTIFY whiteBalanceChanged);
  Q_PROPERTY(ColorTone *colorTone READ colorTone NOTIFY colorToneChanged);
  Q_PROPERTY(Iso *iso READ iso NOTIFY isoChanged);
  Q_PROPERTY(Exposure *exposure READ exposure NOTIFY exposureChanged);
  Q_PROPERTY(Aperture *aperture READ aperture NOTIFY apertureChanged);
  Q_PROPERTY(NoiseReduction *noiseReduction READ noiseReduction NOTIFY noiseReductionChanged);
  Q_PROPERTY(FlickerReduction *flickerReduction READ flickerReduction NOTIFY flickerReductionChanged);
  Q_PROPERTY(Focus *focus READ focus NOTIFY focusChanged);
  Q_PROPERTY(AutoFocus *autoFocus READ autoFocus NOTIFY autoFocusChanged);
  Q_PROPERTY(Roi *roi READ roi NOTIFY roiChanged);

  Q_PROPERTY(VideoMute *videoMute READ videoMute NOTIFY videoMuteChanged);
  Q_PROPERTY(VideoTorch *videoTorch READ videoTorch NOTIFY videoTorchChanged);

  Q_PROPERTY(CameraConfig *cameraConfig READ cameraConfig CONSTANT);

  Q_ENUMS(CameraMode);

public:
  typedef enum {
    UnknownMode,
    ImageMode,
    VideoMode
  } CameraMode;

  Camera(QObject *parent = 0);
  ~Camera();

  int deviceCount() const;
  Q_INVOKABLE QString deviceName(int index) const;
  Q_INVOKABLE QVariant deviceId(int index) const;

  Q_INVOKABLE bool reset(const QVariant& deviceId, const CameraMode& mode);

  QVariant deviceId() const;

  CameraMode mode();

  QtCamDevice *device() const;

  Q_INVOKABLE bool start();
  Q_INVOKABLE bool stop(bool force = false);

  bool isIdle();
  bool isRunning();

  Sounds *sounds() const;
  void setSounds(Sounds *sounds);

  Zoom *zoom() const;
  Flash *flash() const;
  Scene *scene() const;
  EvComp *evComp() const;
  WhiteBalance *whiteBalance() const;
  ColorTone *colorTone() const;
  Iso *iso() const;
  Exposure *exposure() const;
  Aperture *aperture() const;
  NoiseReduction *noiseReduction() const;
  FlickerReduction *flickerReduction() const;
  Focus *focus() const;
  AutoFocus *autoFocus() const;
  Roi *roi() const;

  VideoMute *videoMute() const;
  VideoTorch *videoTorch() const;

  CameraConfig *cameraConfig() const;

signals:
  void deviceCountChanged();
  void deviceIdChanged();
  void prepareForDeviceChange();
  void deviceChanged();
  void modeChanged();
  void idleStateChanged();
  void runningStateChanged();
  void error(const QString& message, int code, const QString& debug);
  void soundsChanged();
  void renderAreaChanged();
  void videoResolutionChanged();

  void zoomChanged();
  void flashChanged();
  void sceneChanged();
  void evCompChanged();
  void whiteBalanceChanged();
  void colorToneChanged();
  void isoChanged();
  void exposureChanged();
  void apertureChanged();
  void noiseReductionChanged();
  void flickerReductionChanged();
  void focusChanged();
  void autoFocusChanged();
  void roiChanged();

  void videoMuteChanged();
  void videoTorchChanged();
  void renderingEnabledChanged();

private:
  bool applyMode();
  bool setDeviceId(const QVariant& deviceId);
  bool setMode(const CameraMode& mode);

  void resetCapabilities();

  CameraConfig *m_config;
  QtCamera *m_cam;
  QtCamDevice *m_dev;
  QVariant m_id;
  CameraMode m_mode;
  NotificationsContainer *m_notifications;

  Zoom *m_zoom;
  Flash *m_flash;
  Scene *m_scene;
  EvComp *m_evComp;
  WhiteBalance *m_whiteBalance;
  ColorTone *m_colorTone;
  Iso *m_iso;
  Exposure *m_exposure;
  Aperture *m_aperture;
  NoiseReduction *m_noiseReduction;
  FlickerReduction *m_flickerReduction;
  Focus *m_focus;
  AutoFocus *m_autoFocus;
  Roi *m_roi;

  VideoMute *m_videoMute;
  VideoTorch *m_videoTorch;
};

#endif /* CAMERA_H */
