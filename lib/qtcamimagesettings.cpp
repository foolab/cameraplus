#include "qtcamimagesettings.h"

class QtCamImageSettingsPrivate {
public:
  QString id;
  QString name;
  QSize capture;
  QSize preview;
  QSize viewfinder;
  int numerator;
  int denominator;
  int nightNumerator;
  int nightDenominator;
};

QtCamImageSettings::QtCamImageSettings(const QString& id, const QString& name,
				       const QSize& capture, const QSize& preview,
				       const QSize& viewfinder,
				       int numerator, int denominator,
				       int nightNumerator, int nightDenominator) :
  d_ptr(new QtCamImageSettingsPrivate) {

  d_ptr->id = id;
  d_ptr->name = name;
  d_ptr->capture = capture;
  d_ptr->preview = preview;
  d_ptr->viewfinder = viewfinder;
  d_ptr->numerator = numerator;
  d_ptr->denominator = denominator;
  d_ptr->nightNumerator = nightNumerator;
  d_ptr->nightDenominator = nightDenominator;
}

QtCamImageSettings::QtCamImageSettings(const QtCamImageSettings& other) :
  d_ptr(new QtCamImageSettingsPrivate) {

  d_ptr->id = other.d_ptr->id;
  d_ptr->name = other.d_ptr->name;
  d_ptr->capture = other.d_ptr->capture;
  d_ptr->preview = other.d_ptr->preview;
  d_ptr->viewfinder = other.d_ptr->viewfinder;
  d_ptr->numerator = other.d_ptr->numerator;
  d_ptr->denominator = other.d_ptr->denominator;
  d_ptr->nightNumerator = other.d_ptr->nightNumerator;
  d_ptr->nightDenominator = other.d_ptr->nightDenominator;
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
  d_ptr->numerator = other.d_ptr->numerator;
  d_ptr->denominator = other.d_ptr->denominator;
  d_ptr->nightNumerator = other.d_ptr->nightNumerator;
  d_ptr->nightDenominator = other.d_ptr->nightDenominator;

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

QPair<int, int> QtCamImageSettings::frameRate() const {
  return qMakePair<int, int>(d_ptr->numerator, d_ptr->denominator);
}

QPair<int, int> QtCamImageSettings::nightFrameRate() const {
  return qMakePair<int, int>(d_ptr->nightNumerator, d_ptr->nightDenominator);
}
