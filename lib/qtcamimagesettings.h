// -*- c++ -*-

#ifndef QT_CAM_IMAGE_SETTINGS_H
#define QT_CAM_IMAGE_SETTINGS_H

#include <QSize>
#include <QPair>
#include <QString>

class QtCamImageSettingsPrivate;

class QtCamImageSettings {
public:
  QtCamImageSettings(const QString& id, const QString& name, const QSize& capture,
		     const QSize& preview, const QSize& viewfinder,
		     int fps, int nightFps);

  QtCamImageSettings(const QtCamImageSettings& other);

  ~QtCamImageSettings();

  QtCamImageSettings& operator=(const QtCamImageSettings& other);

  QString id() const;
  QString name() const;
  QSize captureResolution() const;
  QSize viewfinderResolution() const;
  QSize previewResolution() const;
  int frameRate() const;
  int nightFrameRate() const;

private:
  QtCamImageSettingsPrivate *d_ptr;
};

#endif /* QT_CAM_IMAGE_SETTINGS_H */
