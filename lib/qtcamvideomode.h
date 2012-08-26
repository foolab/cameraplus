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

public:
  QtCamVideoMode(QtCamDevicePrivate *d, QObject *parent = 0);
  ~QtCamVideoMode();

  virtual bool canCapture();
  virtual void applySettings();

  bool isRecording();
  bool startRecording(const QString& fileName);
  bool stopRecording();

  bool setSettings(const QtCamVideoSettings& settings);

  void setProfile(GstEncodingProfile *profile);

protected:
  virtual void start();
  virtual void stop();

private:
  QtCamVideoModePrivate *d_ptr;
};

#endif /* QT_CAM_VIDEO_MODE_H */
