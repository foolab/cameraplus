// -*- c++ -*-

#ifndef QT_CAM_VIDEO_MODE_H
#define QT_CAM_VIDEO_MODE_H

#include "qtcammode.h"
#include <gst/pbutils/encoding-profile.h>

class QtCamDevicePrivate;
class QtCamVideoModePrivate;
class QtCamVideoSettings;

class QtCamVideoMode : public QtCamMode {
  Q_OBJECT

  Q_PROPERTY(bool recording READ isRecording NOTIFY recordingStateChanged);

public:
  QtCamVideoMode(QtCamDevicePrivate *dev, QObject *parent = 0);
  ~QtCamVideoMode();

  virtual bool canCapture();
  virtual void applySettings();

  bool isRecording();
  Q_INVOKABLE bool startRecording(const QString& fileName);

  bool setSettings(const QtCamVideoSettings& settings);

  void setProfile(GstEncodingProfile *profile);

public slots:
  void stopRecording();

signals:
  void recordingStateChanged();

protected:
  virtual void start();
  virtual void stop();

private:
  QtCamVideoModePrivate *d;
  Q_PRIVATE_SLOT(d, void _d_idleStateChanged(bool));
};

#endif /* QT_CAM_VIDEO_MODE_H */
