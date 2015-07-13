/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2014 Mohammed Sameer <msameer@foolab.org>
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

#define DEBUG_GL_READ 0

#include "qtcamviewfinderrenderer.h"
#include "qtcamviewfinderrenderer_p.h"
#include "qtcamconfig.h"
#include <QMap>
#include <QDebug>
#include <GLES2/gl2.h>
#ifdef DEBUG_GL_READ
#include <QElapsedTimer>
#endif

static QMap<QString, QMetaObject> _renderers;

QtCamViewfinderRenderer::QtCamViewfinderRenderer(QtCamConfig *config, QObject *parent) :
  QObject(parent),
  d_ptr(new QtCamViewfinderRendererPrivate) {

  Q_UNUSED(config);
}

QtCamViewfinderRenderer::~QtCamViewfinderRenderer() {
  delete d_ptr;
}

QtCamViewfinderRenderer *QtCamViewfinderRenderer::create(QtCamConfig *config, QObject *parent) {
  QString key = config->viewfinderRenderer();
  if (!_renderers.contains(key)) {
    qCritical() << "Unknown renderer" << key;
    return 0;
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

void QtCamViewfinderRenderer::setViewfinderRotationAngle(int angle) {
  d_ptr->angle = angle;
}

void QtCamViewfinderRenderer::setViewfinderFlipped(bool flipped) {
  d_ptr->flipped = flipped;
}

void QtCamViewfinderRenderer::calculateCoordinates(const QRect& crop, float *coords) {
  int index = d_ptr->angle == 0 ? 0 : d_ptr->angle == -1 ? 0 : 360 / d_ptr->angle;

  qreal tx = 0.0f, ty = 1.0f, sx = 1.0f, sy = 0.0f;

  if (!crop.isEmpty()) {
    QSizeF videoSize = videoResolution();
    int top = crop.y();
    int left = crop.x();
    int right = crop.width() + left;
    int bottom = crop.height() + top;

    if ((right - left) <= 0 || (bottom - top) <= 0) {
      // empty crop rectangle.
      goto out;
    }

    int width = right - left;
    int height = bottom - top;

    int bufferWidth = videoSize.width();
    int bufferHeight = videoSize.height();

    if (width < bufferWidth) {
      tx = (qreal)left / (qreal)bufferWidth;
      sx = (qreal)(left + crop.width()) / (qreal)bufferWidth;
    }

    if (height < bufferHeight) {
      // Our texture is inverted (sensor image Y goes downwards but OpenGL Y goes upwards)
      // so texture coordinate 0,0.75 means crop 25% from the _bottom_
      ty = (qreal)bottom / (qreal)bufferHeight;
      sy = (qreal)(top) / (qreal)bufferHeight;
    }
  }

out:
  float back_coordinates[4][8] = {
    {tx, sy, sx, sy, sx, ty, tx, ty}, // 0
    {0, 0, 1, 0, 1, 1, 0, 1}, // 90       // TODO:
    {sx, ty, tx, ty, tx, sy, sx, sy}, // 180
    {0, 0, 1, 0, 1, 1, 0, 1}, // 270      // TODO:
  };

  // Front has x axis flipped (See note about flipped Y axis above)
  float front_coordinates[4][8] = {
    {sx, sy, tx, sy, tx, ty, sx, ty}, // 0
    {1, 0, 0, 0, 0, 1, 1, 1}, // 90       // TODO:
    {tx, ty, sx, ty, sx, sy, tx, sy}, // 180
    {1, 0, 0, 0, 0, 1, 1, 1}, // 270      // TODO:
  };

  memcpy(coords, d_ptr->flipped ? front_coordinates[index] : back_coordinates[index],
	 8 * sizeof(float));
}

void QtCamViewfinderRenderer::paint(const QMatrix4x4& matrix, const QRectF& viewport) {
  if (!render(matrix, viewport)) {
    return;
  }

  d_ptr->m_lock.lock();
  if (!d_ptr->iface) {
    d_ptr->m_lock.unlock();
    return;
  }

  d_ptr->m_lock.unlock();

  // TODO: this is all Harmattan specific
  QRectF area = renderArea();
  unsigned char *data = (unsigned char *)malloc(area.width() * area.height() * 2);

#if 0
  glFinish();
  glPixelStorei(GL_PACK_ALIGNMENT, 2);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
#endif

#ifdef DEBUG_GL_READ
  QElapsedTimer t;
  t.start();
#endif

  glReadPixels(area.x(), area.y(), area.width(), area.height(),
	       GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);

  GLenum err = glGetError();

#ifdef DEBUG_GL_READ
  qDebug() << err << t.elapsed();
#endif

  if (err != GL_NO_ERROR) {
    qCritical() << "Error" << err << "reading GL pixels";
    free(data);
    return;
  }

  // GL viewport is horizontally mirrored so we need to fix that:
  unsigned char *out_data = (unsigned char *)malloc(area.width() * area.height() * 2);
  int w = area.width();
  int h = area.height();
  for (int x = 0; x < h; x++) {
    unsigned char *dst = &out_data[(h - x - 1) * w * 2];
    memcpy(dst, &data[w*x], w * 2);
  }

  free(data);
  d_ptr->m_lock.lock();

  if (d_ptr->iface) {
    d_ptr->iface->handleData(out_data,
			     QSize(area.width(), area.height()),
			     QtCamViewfinderFrame::RGB565);
  }
  d_ptr->m_lock.unlock();
  free(out_data);
}
