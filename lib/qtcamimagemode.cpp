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
  bool night = d_ptr->inNightMode();

  int fps = night ? d->settings.nightFrameRate() : d->settings.frameRate();

  d_ptr->setCaps("viewfinder-caps", d->settings.viewfinderResolution(), fps);

  // FIXME:
  // Ideally, we should query the image-capture-supported-caps and get a proper framerate
  // as it seems that subdevsrc2 can only capture 15 FPS for at least the highest resolution
  // we use. For now we will not set any FPS.
  d_ptr->setCaps("image-capture-caps", d->settings.captureResolution(), -1);

  setPreviewSize(d->settings.previewResolution());

  // TODO: ?
  // d_ptr->resetCaps("video-capture-caps");
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
