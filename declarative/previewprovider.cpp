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

#include "previewprovider.h"

PreviewProvider *PreviewProvider::m_instance = 0;

PreviewProvider::PreviewProvider() :
  QDeclarativeImageProvider(QDeclarativeImageProvider::Image) {

  m_instance = this;
}

PreviewProvider::~PreviewProvider() {
  m_instance = 0;
}

QImage PreviewProvider::requestImage(const QString& id, QSize *size, const QSize& requestedSize) {
  Q_UNUSED(id);

  QMutexLocker lock(&m_mutex);

  QImage res = m_image;

  if (!requestedSize.isEmpty()) {
    res = res.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  }

  if (size) {
    *size = res.size();
  }

  return res;

}

void PreviewProvider::setPreview(const QImage& preview) {
  QMutexLocker lock(&m_mutex);

  m_image = preview;
}

PreviewProvider *PreviewProvider::instance() {
  return m_instance;
}
