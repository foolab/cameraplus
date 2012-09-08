#include "qtcamvideosettings.h"

class QtCamVideoSettingsPrivate {
public:
  QString id;
  QString name;
  QSize capture;
  QSize preview;
  int fps;
  int nightFps;
};

QtCamVideoSettings::QtCamVideoSettings(const QString& id, const QString& name,
				       const QSize& capture, const QSize& preview,
				       int fps, int nightFps) :
  d_ptr(new QtCamVideoSettingsPrivate) {

  d_ptr->id = id;
  d_ptr->name = name;
  d_ptr->capture = capture;
  d_ptr->preview = preview;
  d_ptr->fps = fps;
  d_ptr->nightFps = nightFps;
}

QtCamVideoSettings::QtCamVideoSettings(const QtCamVideoSettings& other) :
  d_ptr(new QtCamVideoSettingsPrivate) {

  d_ptr->id = other.d_ptr->id;
  d_ptr->name = other.d_ptr->name;
  d_ptr->capture = other.d_ptr->capture;
  d_ptr->preview = other.d_ptr->preview;
  d_ptr->fps = other.d_ptr->fps;
  d_ptr->nightFps = other.d_ptr->nightFps;
}

QtCamVideoSettings::~QtCamVideoSettings() {
  delete d_ptr;
}

QtCamVideoSettings& QtCamVideoSettings::operator=(const QtCamVideoSettings&
								other) {
  d_ptr->id = other.d_ptr->id;
  d_ptr->name = other.d_ptr->name;
  d_ptr->capture = other.d_ptr->capture;
  d_ptr->preview = other.d_ptr->preview;
  d_ptr->fps = other.d_ptr->fps;
  d_ptr->nightFps = other.d_ptr->nightFps;

  return *this;
}

QString QtCamVideoSettings::id() const {
  return d_ptr->id;
}

QString QtCamVideoSettings::name() const {
  return d_ptr->name;
}

QSize QtCamVideoSettings::captureResolution() const {
  return d_ptr->capture;
}

QSize QtCamVideoSettings::previewResolution() const {
  return d_ptr->preview;
}

int QtCamVideoSettings::frameRate() const {
  return d_ptr->fps;
}

int QtCamVideoSettings::nightFrameRate() const {
  return d_ptr->nightFps;
}
