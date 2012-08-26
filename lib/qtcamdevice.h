// -*- c++ -*-

#ifndef QT_CAM_DEVICE_H
#define QT_CAM_DEVICE_H

#include <QObject>
#include <QVariant>

class QtCamDevicePrivate;
class QtCamConfig;
class QtCamViewfinder;
class QtCamVideoMode;
class QtCamImageMode;
class QtCamMode;
class QtCamGStreamerMessageListener;
class QtCamMetaData;

class QtCamDevice : public QObject {
  Q_OBJECT

public:
  QtCamDevice(QtCamConfig *config, const QString& name, const QVariant& id, QObject *parent = 0);
  ~QtCamDevice();

  bool setViewfinder(QtCamViewfinder *viewfinder);
  bool start();
  bool stop();
  bool isRunning();
  bool isIdle();

  QtCamImageMode *imageMode() const;
  QtCamVideoMode *videoMode() const;

  QtCamMode *activeMode() const;

  QString name() const;
  QVariant id() const;

  QtCamConfig *config() const;
  QtCamGStreamerMessageListener *listener() const;

signals:
  void error(const QString& message, int code, const QString& debug);
  void started();
  void stopped();

private:
  Q_PRIVATE_SLOT(d_ptr, void _d_error(const QString&, int, const QString&))
  friend class QtCamMetaData;
  QtCamDevicePrivate *d_ptr;
};

#endif /* QT_CAM_DEVICE_H */
