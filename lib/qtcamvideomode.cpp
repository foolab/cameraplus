#include "qtcamvideomode.h"
#include "qtcammode_p.h"
#include <QDebug>
#include "qtcamdevice_p.h"
#include "qtcamdevice.h"
#include "qtcamvideosettings.h"

class QtCamVideoModePrivate : public QtCamModePrivate {
public:
  QtCamVideoModePrivate(QtCamDevicePrivate *dev) :
  QtCamModePrivate(dev),
  settings(dev->conf->defaultVideoSettings()) {

  }

  ~QtCamVideoModePrivate() {}

  QtCamVideoSettings settings;
};

QtCamVideoMode::QtCamVideoMode(QtCamDevicePrivate *d, QObject *parent) :
  QtCamMode(new QtCamVideoModePrivate(d), "mode-video", "video-done", parent) {

  d_ptr = (QtCamVideoModePrivate *)QtCamMode::d_ptr;

  QString name = d_ptr->dev->conf->videoEncodingProfileName();
  QString path = d_ptr->dev->conf->videoEncodingProfilePath();

  if (!name.isEmpty() && !path.isEmpty()) {
    GstEncodingProfile *profile = d_ptr->loadProfile(path, name);
    if (profile) {
      setProfile(profile);
    }
  }
}

QtCamVideoMode::~QtCamVideoMode() {

}

bool QtCamVideoMode::canCapture() {
  return d_ptr->dev->q_ptr->isIdle();
}

void QtCamVideoMode::applySettings() {
  setCaps("video-capture-caps", d_ptr->settings.captureResolution(),
	  d_ptr->settings.frameRate());
  setCaps("viewfinder-caps", d_ptr->settings.captureResolution(),
  	  d_ptr->settings.frameRate());
  setPreviewSize(d_ptr->settings.previewResolution());
}

void QtCamVideoMode::start() {
  // Nothing
}

void QtCamVideoMode::stop() {
  if (isRecording()) {
    stopRecording();
  }
}

bool QtCamVideoMode::isRecording() {
  return !d_ptr->dev->q_ptr->isIdle();
}

bool QtCamVideoMode::startRecording(const QString& fileName) {
  if (!canCapture() || isRecording()) {
    return false;
  }

  setFileName(fileName);

  g_object_set(d_ptr->dev->cameraBin, "location", fileName.toUtf8().data(), NULL);
  g_signal_emit_by_name(d_ptr->dev->cameraBin, "start-capture", NULL);

  return true;
}

bool QtCamVideoMode::stopRecording() {
  if (isRecording()) {
    g_signal_emit_by_name(d_ptr->dev->cameraBin, "stop-capture", NULL);
  }

  return true;
}

bool QtCamVideoMode::setSettings(const QtCamVideoSettings& settings) {
  d_ptr->settings = settings;

  if (isRecording()) {
    return false;
  }

  applySettings();

  return true;
}


void QtCamVideoMode::setProfile(GstEncodingProfile *profile) {
  if (!d_ptr->dev->cameraBin) {
    gst_encoding_profile_unref(profile);
    return;
  }

  g_object_set(d_ptr->dev->cameraBin, "video-profile", profile, NULL);
}
