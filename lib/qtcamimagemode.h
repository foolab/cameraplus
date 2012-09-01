// -*- c++ -*-

#ifndef QT_CAM_IMAGE_MODE_H
#define QT_CAM_IMAGE_MODE_H

#include "qtcammode.h"
#include <gst/pbutils/encoding-profile.h>

class QtCamDevicePrivate;
class QtCamImageModePrivate;
class QtCamImageSettings;

class QtCamImageMode : public QtCamMode {
  Q_OBJECT

public:
  QtCamImageMode(QtCamDevicePrivate *dev, QObject *parent = 0);
  ~QtCamImageMode();

  virtual bool canCapture();
  virtual void applySettings();

  bool capture(const QString& fileName);

  bool setSettings(const QtCamImageSettings& settings);

  void setProfile(GstEncodingProfile *profile);

protected:
  virtual void start();
  virtual void stop();

private:
  QtCamImageModePrivate *d;
};

#endif /* QT_CAM_IMAGE_MODE_H */
