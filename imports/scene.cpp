#include "scene.h"
#include "camera.h"
#include <QDebug>

Scene::Scene(QObject *parent) :
  Capability(parent),
  m_scene(0) {

}

Scene::~Scene() {
  if (m_scene) {
    delete m_scene; m_scene = 0;
  }
}

void Scene::deviceChanged() {
  if (m_scene) {
    delete m_scene; m_scene = 0;
  }

  if (m_cam->device()) {
    m_scene = new QtCamScene(m_cam->device(), this);
    QObject::connect(m_scene, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
  }

  emit valueChanged();
}

Scene::SceneMode Scene::value() {
  return m_scene ? (SceneMode)m_scene->value() : Scene::Auto;
}

void Scene::setValue(const Scene::SceneMode& mode) {
  if (m_scene) {
    m_scene->setValue((QtCamScene::SceneMode)mode);
  }
}
