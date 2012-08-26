// -*- c++ -*-

#ifndef MAIN_H
#define MAIN_H

#include <QWidget>

class QtCamera;
class QtCamDevice;
class QtCamGraphicsViewfinder;
class QPushButton;
class QtCamMetaData;
class QGraphicsScene;

class Camera : public QWidget {
  Q_OBJECT

public:
  Camera(QWidget *parent = 0);
  ~Camera();

private slots:
  void setDevice(QAction *action);
  void switchToImage();
  void switchToVideo();
  void buttonClicked();

private:
  QGraphicsScene *m_scene;
  QtCamera *m_cam;
  QtCamDevice *m_dev;
  QtCamGraphicsViewfinder *m_vf;
  QPushButton *m_button;
  QtCamMetaData *m_meta;
};

#endif /* MAIN_H */
