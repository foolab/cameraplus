// -*- c++ -*-

#ifndef QT_CAM_MODE_H
#define QT_CAM_MODE_H

#include <QObject>
#include <QPair>

class QtCamModePrivate;
class QtCamDevicePrivate;
class QSize;
class QImage;

class QtCamMode : public QObject {
  Q_OBJECT

public:
  QtCamMode(QtCamModePrivate *d, const char *mode, const char *done, QObject *parent = 0);
  virtual ~QtCamMode();

  void activate();
  void deactivate();

  virtual bool canCapture();
  bool isActive();

  virtual void applySettings() = 0;

signals:
  void previewAvailable(const QImage& image, const QString& fileName);
  void saved(const QString& fileName);

protected:
  virtual void start() = 0;
  virtual void stop() = 0;

  void setCaps(const char *property, const QSize& resolution, const QPair<int, int> frameRate);
  void setPreviewSize(const QSize& size);

  void setFileName(const QString& fileName);

  QtCamModePrivate *d_ptr;
};

#endif /* QT_CAM_MODE_H */
