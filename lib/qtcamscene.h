// -*- c++ -*-

#ifndef QT_CAM_SCENE_H
#define QT_CAM_SCENE_H

#include "qtcamcapability.h"

class QtCamScene : public QtCamCapability {
  Q_OBJECT

public:
  typedef enum {
    Manual = 0,
    Closeup = 1,
    Portrait = 2,
    Landscape = 3,
    Sport = 4,
    Night = 5,
    Auto = 6
  } SceneMode;

  QtCamScene(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamScene();

  SceneMode value();
  bool setValue(const SceneMode& mode);
};

#endif /* QT_CAM_SCENE_H */
