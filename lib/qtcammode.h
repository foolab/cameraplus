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

  Q_PROPERTY(bool canCapture READ canCapture NOTIFY canCaptureChanged);
  Q_PROPERTY(bool active READ isActive NOTIFY activeChanged);

public:
  QtCamMode(QtCamModePrivate *d, const char *mode, const char *done, QObject *parent = 0);
  virtual ~QtCamMode();

  void deactivate();

  virtual bool canCapture();

  bool isActive();

  virtual void applySettings() = 0;

public slots:
  void activate();

signals:
  void previewAvailable(const QImage& image, const QString& fileName);
  void saved(const QString& fileName);
  void canCaptureChanged();
  void activeChanged();

protected:
  virtual void start() = 0;
  virtual void stop() = 0;

  void setPreviewSize(const QSize& size);

  void setFileName(const QString& fileName);

  QtCamModePrivate *d_ptr;
};

#endif /* QT_CAM_MODE_H */
