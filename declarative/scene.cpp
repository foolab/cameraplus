/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "scene.h"

Scene::Scene(QtCamDevice *dev, QObject *parent) :
  QObject(parent),
  m_scene(new QtCamScene(dev, this)) {

  QObject::connect(m_scene, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
}

Scene::~Scene() {
  delete m_scene; m_scene = 0;
}

Scene::SceneMode Scene::value() const {
  return (SceneMode)m_scene->value();
}

void Scene::setValue(const Scene::SceneMode& mode) {
  m_scene->setValue((QtCamScene::SceneMode)mode);
}
