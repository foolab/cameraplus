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

#ifndef PREVIEW_PROVIDER_H
#define PREVIEW_PROVIDER_H

#if defined(QT4)
#include <QDeclarativeImageProvider>
#elif defined(QT5)
#include <QQuickImageProvider>
#endif
#include <QMutex>

#if defined(QT4)
class PreviewProvider : public QDeclarativeImageProvider {
#elif defined(QT5)
class PreviewProvider : public QQuickImageProvider {
#endif

public:
  PreviewProvider();
  ~PreviewProvider();

  static PreviewProvider *instance();

  virtual QImage requestImage(const QString& id, QSize *size, const QSize& requestedSize);
  void setPreview(const QImage& preview);

private:
  static PreviewProvider *m_instance;
  QImage m_image;
  QMutex m_mutex;
};

#endif /* PREVIEW_PROVIDER_H */
