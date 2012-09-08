#include "qtcamimagesettings.h"

class QtCamImageSettingsPrivate {
public:
  QString id;
  QString name;
  QSize capture;
  QSize preview;
  QSize viewfinder;
  int fps;
  int nightFps;
};

QtCamImageSettings::QtCamImageSettings(const QString& id, const QString& name,
				       const QSize& capture, const QSize& preview,
				       const QSize& viewfinder,
				       int fps, int nightFps) :
  d_ptr(new QtCamImageSettingsPrivate) {

  d_ptr->id = id;
  d_ptr->name = name;
  d_ptr->capture = capture;
  d_ptr->preview = preview;
  d_ptr->viewfinder = viewfinder;
  d_ptr->fps = fps;
  d_ptr->nightFps = nightFps;
}

QtCamImageSettings::QtCamImageSettings(const QtCamImageSettings& other) :
  d_ptr(new QtCamImageSettingsPrivate) {

  d_ptr->id = other.d_ptr->id;
  d_ptr->name = other.d_ptr->name;
  d_ptr->capture = other.d_ptr->capture;
  d_ptr->preview = other.d_ptr->preview;
  d_ptr->viewfinder = other.d_ptr->viewfinder;
  d_ptr->fps = other.d_ptr->fps;
  d_ptr->nightFps = other.d_ptr->nightFps;
}

QtCamImageSettings::~QtCamImageSettings() {
  delete d_ptr;
}

QtCamImageSettings& QtCamImageSettings::operator=(const QtCamImageSettings&
								other) {
  d_ptr->id = other.d_ptr->id;
  d_ptr->name = other.d_ptr->name;
  d_ptr->capture = other.d_ptr->capture;
  d_ptr->preview = other.d_ptr->preview;
  d_ptr->viewfinder = other.d_ptr->viewfinder;
  d_ptr->fps = other.d_ptr->fps;
  d_ptr->nightFps = other.d_ptr->nightFps;

  return *this;
}

QString QtCamImageSettings::id() const {
  return d_ptr->id;
}

QString QtCamImageSettings::name() const {
  return d_ptr->name;
}

QSize QtCamImageSettings::captureResolution() const {
  return d_ptr->capture;
}

QSize QtCamImageSettings::viewfinderResolution() const {
  return d_ptr->viewfinder;
}

QSize QtCamImageSettings::previewResolution() const {
  return d_ptr->preview;
}

int QtCamImageSettings::frameRate() const {
  return d_ptr->fps;
}

int QtCamImageSettings::nightFrameRate() const {
  return d_ptr->nightFps;
}
