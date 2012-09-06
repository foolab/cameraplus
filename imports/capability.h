// -*- c++ -*-

#ifndef CAPABILITY_H
#define CAPABILITY_H

#include <QObject>

class Camera;

class Capability : public QObject {
  Q_OBJECT
  Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged);
  Q_PROPERTY(bool ready READ isReady NOTIFY isReadyChanged);

public:
  Capability(QObject *parent = 0);
  virtual ~Capability();

  Camera *camera();
  void setCamera(Camera *cam);

  bool isReady() const;

signals:
  void cameraChanged();
  void isReadyChanged();

private slots:
  virtual void deviceChanged() = 0;

protected:
  Camera *m_cam;
};

#endif /* CAPABILITY_H */
