#include "qtcamconfig.h"
#include <QSettings>
#include <QStringList>
#include <QDebug>

#define CONFIGURATION_FILE                    "/etc/qtcamera.ini"

class QtCamConfigPrivate {
public:
  QString element(const QString& name) const {
    return conf->value(QString("%1/element").arg(name)).toString();
  }

  QSize readResolution(const QString key) {
    QList<QString> parts = conf->value(key).toString().trimmed().split("x");
    return QSize(parts[0].toInt(), parts[1].toInt());
  }

  QSettings *conf;

  QList<QtCamImageSettings> imageSettings;
  QList<QtCamVideoSettings> videoSettings;
};

QtCamConfig::QtCamConfig(QObject *parent) :
  QObject(parent), d_ptr(new QtCamConfigPrivate) {

  d_ptr->conf = new QSettings(CONFIGURATION_FILE, QSettings::IniFormat, this);
}

QtCamConfig::QtCamConfig(const QString& configPath, QObject *parent) :
  QObject(parent), d_ptr(new QtCamConfigPrivate) {

  d_ptr->conf = new QSettings(configPath, QSettings::IniFormat, this);

  defaultImageSettings();
}

QtCamConfig::~QtCamConfig() {
  delete d_ptr;
}

QString QtCamConfig::deviceScannerType() const {
  return d_ptr->conf->value("devices/scanner").toString();
}

QString QtCamConfig::deviceScannerProperty() const {
  return d_ptr->conf->value("devices/property").toString();
}

QString QtCamConfig::videoSource() const {
  return d_ptr->element("video-source");
}

QString QtCamConfig::viewfinderSink() const {
  return d_ptr->element("viewfinder-sink");
}

QString QtCamConfig::viewfinderRenderer() const {
  return d_ptr->conf->value("viewfinder-sink/renderer").toString();
}

QString QtCamConfig::audioSource() const {
  return d_ptr->element("audio-source");
}

QString QtCamConfig::wrapperVideoSource() const {
  return d_ptr->element("wrapper-video-source");
}

QString QtCamConfig::wrapperVideoSourceProperty() const {
  return d_ptr->conf->value("wrapper-video-source/property").toString();
}

QtCamImageSettings QtCamConfig::defaultImageSettings() {

  QList<QtCamImageSettings> settings = imageSettings();

  QString def = d_ptr->conf->value("image/default").toString();
  foreach (const QtCamImageSettings& s, settings) {
    if (s.id() == def) {
      return s;
    }
  }

  // This will crash if no presets have been shipped but you deserve paying
  // the price of shipping a broken configuration file.
  return settings[0];
}

QList<QtCamImageSettings> QtCamConfig::imageSettings() {
  if (d_ptr->imageSettings.isEmpty()) {
    QStringList presets = d_ptr->conf->value("image/presets").toStringList();
    foreach (const QString& preset, presets) {
      d_ptr->conf->beginGroup(preset);

      d_ptr->imageSettings <<
	QtCamImageSettings(preset, d_ptr->conf->value("name").toString(),
			   d_ptr->readResolution("capture"),
			   d_ptr->readResolution("preview"),
			   d_ptr->readResolution("viewfinder"),
			   d_ptr->conf->value("fps").toInt(),
			   d_ptr->conf->value("night").toInt());

      d_ptr->conf->endGroup();
    }
  }

  return d_ptr->imageSettings;
}

QtCamVideoSettings QtCamConfig::defaultVideoSettings() {
  QList<QtCamVideoSettings> settings = videoSettings();

  QString def = d_ptr->conf->value("video/default").toString();
  foreach (const QtCamVideoSettings& s, settings) {
    if (s.id() == def) {
      return s;
    }
  }

  // This will crash if no presets have been shipped but you deserve paying
  // the price of shipping a broken configuration file.
  return settings[0];
}

QList<QtCamVideoSettings> QtCamConfig::videoSettings() {
  if (d_ptr->videoSettings.isEmpty()) {
    QStringList presets = d_ptr->conf->value("video/presets").toStringList();
    foreach (const QString& preset, presets) {
      d_ptr->conf->beginGroup(preset);

      d_ptr->videoSettings <<
	QtCamVideoSettings(preset, d_ptr->conf->value("name").toString(),
			   d_ptr->readResolution("capture"),
			   d_ptr->readResolution("preview"),
			   d_ptr->conf->value("fps").toInt(),
			   d_ptr->conf->value("night").toInt());

      d_ptr->conf->endGroup();
    }
  }

  return d_ptr->videoSettings;
}

QString QtCamConfig::imageEncodingProfileName() const {
  return d_ptr->conf->value("image/profile-name").toString();
}

QString QtCamConfig::imageEncodingProfilePath() const {
  return d_ptr->conf->value("image/profile-path").toString();
}

QString QtCamConfig::videoEncodingProfileName() const {
  return d_ptr->conf->value("video/profile-name").toString();
}

QString QtCamConfig::videoEncodingProfilePath() const {
  return d_ptr->conf->value("video/profile-path").toString();
}

QString QtCamConfig::audioCaptureCaps() const {
  return d_ptr->conf->value("audio-capture-caps/caps").toString();
}

QString QtCamConfig::imageSuffix() const {
  return d_ptr->conf->value("image/extension").toString();
}

QString QtCamConfig::videoSuffix() const {
  return d_ptr->conf->value("video/extension").toString();
}
