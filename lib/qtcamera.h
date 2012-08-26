// -*- c++ -*-

#ifndef QT_CAMERA_H
#define QT_CAMERA_H

#include <QObject>
#include <QPair>
#include <QVariant>

class QtCamConfig;
class QtCameraPrivate;
class QtCamDevice;

class QtCamera : public QObject {
  Q_OBJECT

public:
  QtCamera(QObject *parent = 0);
  QtCamera(const QString& configPath, QObject *parent = 0);
  QtCamera(QtCamConfig *config, QObject *parent = 0);

  ~QtCamera();

  void refreshDevices();

  QList<QPair<QString, QVariant> > devices() const;

  QtCamDevice *device(const QVariant& id, QObject *parent = 0);

  QtCamConfig *config() const;

private:
  QtCameraPrivate *d_ptr;
};

#endif /* QT_CAMERA_H */
