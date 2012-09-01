#include "qtcamimagemode.h"
#include "qtcammode_p.h"
#include "qtcamdevice_p.h"
#include "qtcamimagesettings.h"
#include "qtcamdevice.h"

class QtCamImageModePrivate : public QtCamModePrivate {
public:
  QtCamImageModePrivate(QtCamDevicePrivate *dev) :
  QtCamModePrivate(dev),
  settings(dev->conf->defaultImageSettings()) {

  }

  ~QtCamImageModePrivate() {
  }

  QtCamImageSettings settings;
};

QtCamImageMode::QtCamImageMode(QtCamDevicePrivate *dev, QObject *parent) :
  QtCamMode(new QtCamImageModePrivate(dev), "mode-image", "image-done", parent) {

  d = (QtCamImageModePrivate *)d_ptr;

  QString name = d_ptr->dev->conf->imageEncodingProfileName();
  QString path = d_ptr->dev->conf->imageEncodingProfilePath();

  if (!name.isEmpty() && !path.isEmpty()) {
    GstEncodingProfile *profile = d_ptr->loadProfile(path, name);
    if (profile) {
      setProfile(profile);
    }
  }
}

QtCamImageMode::~QtCamImageMode() {
  d = 0;
}

bool QtCamImageMode::canCapture() {
  return QtCamMode::canCapture() && d_ptr->dev->isWrapperReady();
}

void QtCamImageMode::applySettings() {
  d_ptr->setCaps("image-capture-caps", d->settings.captureResolution(),
		 d->settings.frameRate());
  d_ptr->setCaps("viewfinder-caps", d->settings.viewfinderResolution(),
		 d->settings.frameRate());

  setPreviewSize(d->settings.previewResolution());
}

void QtCamImageMode::start() {
  // Nothing
}

void QtCamImageMode::stop() {
  // Nothing
}

bool QtCamImageMode::capture(const QString& fileName) {
  if (!canCapture()) {
    return false;
  }

  setFileName(fileName);

  g_object_set(d_ptr->dev->cameraBin, "location", fileName.toUtf8().data(), NULL);
  g_signal_emit_by_name(d_ptr->dev->cameraBin, "start-capture", NULL);

  return true;
}

bool QtCamImageMode::setSettings(const QtCamImageSettings& settings) {
  d->settings = settings;

  if (!d_ptr->dev->q_ptr->isIdle()) {
    return false;
  }

  applySettings();

  return true;
}

void QtCamImageMode::setProfile(GstEncodingProfile *profile) {
  if (!d_ptr->dev->cameraBin) {
    gst_encoding_profile_unref(profile);
    return;
  }

  g_object_set(d_ptr->dev->cameraBin, "image-profile", profile, NULL);
}
