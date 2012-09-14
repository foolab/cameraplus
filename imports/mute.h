// -*- c++ -*-
#ifndef MUTE_H
#define MUTE_H

#include <QObject>

class Camera;
class QtCamMute;

class Mute : public QObject {
  Q_OBJECT
  Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged);
  Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY stateChanged);

public:
  Mute(QObject *parent = 0);
  ~Mute();

  Camera *camera();
  void setCamera(Camera *camera);

  bool isEnabled() const;
  void setEnabled(bool enabled);

signals:
  void stateChanged();
  void cameraChanged();

private slots:
  void deviceChanged();

private:
  Camera *m_cam;
  QtCamMute *m_mute;
};

#endif /* MUTE_H */
