// -*- c++ -*-

#ifndef QT_CAM_VIDEO_SETTINGS_H
#define QT_CAM_VIDEO_SETTINGS_H

#include <QSize>
#include <QPair>
#include <QString>

class QtCamVideoSettingsPrivate;

class QtCamVideoSettings {
public:
  QtCamVideoSettings(const QString& id, const QString& name,
		     const QSize& capture, const QSize& preview,
		     int numerator, int denominator);

  QtCamVideoSettings(const QtCamVideoSettings& other);

  ~QtCamVideoSettings();

  QtCamVideoSettings& operator=(const QtCamVideoSettings& other);

  QString id() const;
  QString name() const;
  QSize captureResolution() const;
  QSize previewResolution() const;
  QPair<int, int> frameRate() const;

private:
  QtCamVideoSettingsPrivate *d_ptr;
};

#endif /* QT_CAM_VIDEO_SETTINGS_H */
