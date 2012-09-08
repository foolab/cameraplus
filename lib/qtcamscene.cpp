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
  SceneMode old = value();

  // Scene mode is always forced in order to reset the other capabilities.
  bool ret = d_ptr->setIntValue(mode, true);

  if (!ret) {
    return ret;
  }

  if (!d_ptr->dev || !d_ptr->dev->activeMode() || old == mode) {
    return ret;
  }

  if (old == Night || mode == Night) {
    // We must ask the mode to reset the settings to use night mode resolution if needed.
    d_ptr->dev->activeMode()->applySettings();
  }

  return ret;
}
