// -*- c++ -*-

#ifndef QT_CAM_CONFIG_H
#define QT_CAM_CONFIG_H

#include <QObject>
#include "qtcamimagesettings.h"
#include "qtcamvideosettings.h"

#define SCANNER_TYPE_V4L2                     "v4l2"
#define SCANNER_TYPE_ENUM                     "enum"

// TODO: kill those
//#define RENDERER_TYPE_GL_SINK                 "glsink"
//#define RENDERER_TYPE_X_OVERLAY               "xoverlay"
#define RENDERER_TYPE_MEEGO                   "meego"
#define RENDERER_TYPE_GENERIC                 "generic"
class QtCamConfigPrivate;

class QtCamConfig : public QObject {
  Q_OBJECT

public:
  QtCamConfig(QObject *parent = 0);
  QtCamConfig(const QString& configPath, QObject *parent = 0);

  virtual ~QtCamConfig();

  QString deviceScannerType() const;
  QString deviceScannerProperty() const;

  QString videoSource() const;
  QString viewfinderSink() const;
  QString viewfinderRenderer() const;
  QString audioSource() const;
  QString wrapperVideoSource() const;
  QString wrapperVideoSourceProperty() const;

  QtCamImageSettings defaultImageSettings();
  QList<QtCamImageSettings> imageSettings();

  QtCamVideoSettings defaultVideoSettings();
  QList<QtCamVideoSettings> videoSettings();

  QString imageEncodingProfileName() const;
  QString imageEncodingProfilePath() const;

  QString videoEncodingProfileName() const;
  QString videoEncodingProfilePath() const;

  QString imageSuffix() const;
  QString videoSuffix() const;

  QString audioCaptureCaps() const;

private:
  QtCamConfigPrivate *d_ptr;
};

#endif /* QT_CAM_CONFIG_H */
