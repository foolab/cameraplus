// -*- c++ -*-

#ifndef CAPABILITY_H
#define CAPABILITY_H

#include <QObject>

class Camera;

class Capability : public QObject {
  Q_OBJECT
  Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged);

public:
  Capability(QObject *parent = 0);
  virtual ~Capability();

  Camera *camera();
  void setCamera(Camera *cam);

signals:
  void cameraChanged();

private slots:
  virtual void deviceChanged() = 0;

protected:
  Camera *m_cam;
};

#endif /* CAPABILITY_H */
