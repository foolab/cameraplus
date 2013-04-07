// -*- c++ -*-

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

#ifndef SCENE_H
#define SCENE_H

#include "capability.h"
#include "qtcamscene.h"

class QtCamDevice;

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

  Scene(QtCamDevice *dev, QObject *parent = 0);
  ~Scene();

  SceneMode value() const;
  void setValue(const SceneMode& mode);

signals:
  void valueChanged();

private:
  QtCamScene *m_scene;
};

#endif /* SCENE_H */
