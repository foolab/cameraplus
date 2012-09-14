// -*- c++ -*-

#ifndef CAMERA_H
#define CAMERA_H

#include <QDeclarativeItem>
#include <QVariant>

class QtCamera;
class QtCamDevice;
class QtCamGraphicsViewfinder;

class Camera : public QDeclarativeItem {
  Q_OBJECT

  Q_PROPERTY(int deviceCount READ deviceCount NOTIFY deviceCountChanged)
  Q_PROPERTY(QVariant deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged);
  Q_PROPERTY(CameraMode mode READ mode WRITE setMode NOTIFY modeChanged);
  Q_PROPERTY(bool idle READ isIdle NOTIFY idleStateChanged);
  Q_PROPERTY(bool running READ isRunning NOTIFY runningStateChanged);
  Q_PROPERTY(QString imageSuffix READ imageSuffix CONSTANT);
  Q_PROPERTY(QString videoSuffix READ videoSuffix CONSTANT);
  Q_ENUMS(CameraMode);

public:
  typedef enum {
    ImageMode,
    VideoMode
  } CameraMode;

  Camera(QDeclarativeItem *parent = 0);
  ~Camera();

  virtual void componentComplete();

  int deviceCount() const;
  Q_INVOKABLE QString deviceName(int index) const;
  Q_INVOKABLE QVariant deviceId(int index) const;

  void setDeviceId(const QVariant& id);
  QVariant deviceId() const;

  void setMode(const CameraMode& mode);
  CameraMode mode();

  QtCamDevice *device() const;

  Q_INVOKABLE bool start();
  Q_INVOKABLE bool stop();

  bool isIdle();
  bool isRunning();

  QString imageSuffix() const;
  QString videoSuffix() const;

signals:
  void deviceCountChanged();
  void deviceIdChanged();
  void deviceChanged();
  void modeChanged();
  void idleStateChanged();
  void runningStateChanged();
  void error(const QString& message, int code, const QString& debug);

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

private:
  void applyMode();

  QtCamera *m_cam;
  QtCamDevice *m_dev;
  QVariant m_id;
  QtCamGraphicsViewfinder *m_vf;
  CameraMode m_mode;
};

#endif /* CAMERA_H */
