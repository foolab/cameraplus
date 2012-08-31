#include "qtcamscene.h"
#include "qtcamcapability_p.h"

QtCamScene::QtCamScene(QtCamDevice *dev, QObject *parent) :
  QtCamCapability(new QtCamCapabilityPrivate(dev, QtCamCapability::Scene, "scene-mode"), parent) {

}

QtCamScene::~QtCamScene() {

}

QtCamScene::SceneMode QtCamScene::value() {
  int val = 0;
  if (!d_ptr->intValue(&val)) {
    return QtCamScene::Auto;
  }

  switch (val) {
  case QtCamScene::Manual:
  case QtCamScene::Closeup:
  case QtCamScene::Portrait:
  case QtCamScene::Landscape:
  case QtCamScene::Sport:
  case QtCamScene::Night:
    return (QtCamScene::SceneMode)val;

  default:
    return QtCamScene::Auto;
  }
}

bool QtCamScene::setValue(const QtCamScene::SceneMode& mode) {
  return d_ptr->setIntValue(mode);
}
