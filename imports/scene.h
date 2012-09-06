// -*- c++ -*-

#ifndef SCENE_H
#define SCENE_H

#include "capability.h"
#include "qtcamscene.h"

class Scene : public Capability {
  Q_OBJECT

  Q_PROPERTY(SceneMode value READ value WRITE setValue NOTIFY valueChanged);
  Q_ENUMS(SceneMode);

public:
  typedef enum {
    Manual = QtCamScene::Manual,
    Closeup = QtCamScene::Closeup,
    Portrait = QtCamScene::Portrait,
    Landscape = QtCamScene::Landscape,
    Sport = QtCamScene::Sport,
    Night = QtCamScene::Night,
    Auto = QtCamScene::Auto
  } SceneMode;

  Scene(QObject *parent = 0);
  ~Scene();

  SceneMode value();
  void setValue(const SceneMode& mode);

signals:
  void valueChanged();

private:
  virtual void deviceChanged();

  QtCamScene *m_scene;
};

#endif /* SCENE_H */
