/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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

#include "qtcamviewfinderrenderer.h"
#include "qtcamconfig.h"
#include <QMap>
#include <QDebug>

static QMap<QString, QMetaObject> _renderers;

QtCamViewfinderRenderer::QtCamViewfinderRenderer(QtCamConfig *config, QObject *parent) :
  QObject(parent) {

  Q_UNUSED(config);
}

QtCamViewfinderRenderer::~QtCamViewfinderRenderer() {

}

QtCamViewfinderRenderer *QtCamViewfinderRenderer::create(QtCamConfig *config, QObject *parent) {
  QString key = config->viewfinderRenderer();
  if (!_renderers.contains(key)) {
    qCritical() << "Unknown renderer" << key << "will fallback to" << RENDERER_TYPE_GENERIC;
    key = RENDERER_TYPE_GENERIC;
  }

  QObject *obj = _renderers[key].newInstance(Q_ARG(QtCamConfig *, config),
					     Q_ARG(QObject *, parent));

  if (!obj) {
    qCritical() << "Failed to create renderer" << key;
    return 0;
  }

  return dynamic_cast<QtCamViewfinderRenderer *>(obj);
}

int QtCamViewfinderRenderer::registerRenderer(const QString& key, const QMetaObject& meta) {
  _renderers[key] = meta;
  return _renderers.size() - 1;
}
