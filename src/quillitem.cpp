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

#include "quillitem.h"
#include <QuillFile>
#include <QUrl>
#include <QPainter>
#include <QDir>

#if defined(QT5)
QuillItem::QuillItem(QQuickItem *parent) :
  QQuickPaintedItem(parent),
#else
QuillItem::QuillItem(QDeclarativeItem *parent) :
  QDeclarativeItem(parent),
#endif
  m_file(0),
  m_error(false),
  m_displayLevel(-1) {

#if defined(QT4)
  setFlag(QGraphicsItem::ItemHasNoContents, false);
#endif

}

QuillItem::~QuillItem() {
  delete m_file; m_file = 0;
}

void QuillItem::initialize(const QUrl& url, const QString& mimeType, int displayLevel) {
  if (!url.isValid()) {
    return;
  }

  m_displayLevel = displayLevel;

  if (m_error) {
    m_error = false;
    emit errorChanged();
  }

  if (m_file) {
    m_file->deleteLater();
  }

  m_file = new QuillFile(url.toLocalFile(), mimeType);
  m_file->setPriority(displayLevel == QuillItem::DisplayLevelFullScreen ?
		      QuillFile::Priority_High : QuillFile::Priority_Low);

  QObject::connect(m_file, SIGNAL(error(QuillError)),
		   this, SLOT(fileError()), Qt::QueuedConnection);
  QObject::connect(m_file, SIGNAL(imageAvailable(QuillImageList)),
		   this, SLOT(fileLoaded()), Qt::QueuedConnection);
  QObject::connect(m_file, SIGNAL(removed()),
	   m_file, SLOT(deleteLater()), Qt::QueuedConnection);

  if (fileError()) {
    return;
  }

  m_file->setDisplayLevel(displayLevel);

  if (fileError()) {
    return;
  }

  // We call this just in case we are using 1 file with 2 display levels
  update();
}

void QuillItem::clear() {
  if (m_file) {
    m_file->deleteLater();
    m_file = 0;
  }

  update();
}

bool QuillItem::error() const {
  return m_error;
}

#if defined(QT4)
void QuillItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
  Q_UNUSED(widget);
  Q_UNUSED(option);
#elif defined(QT5)
void QuillItem::paint(QPainter* painter) {
#endif

  QRectF rect = boundingRect();
  painter->fillRect(rect, Qt::black);

  if (!m_file) {
    return;
  }

  QImage image = m_file->image(m_displayLevel);

  if (image.isNull()) {
    return;
  }

  QSizeF imageSize = QSizeF(image.size());
  QSizeF widgetSize = rect.size();
  imageSize.scale(widgetSize, Qt::KeepAspectRatio);

  QPointF pos = QPointF(widgetSize.width() - imageSize.width(),
			widgetSize.height() - imageSize.height()) / 2;

  painter->drawImage(QRectF(pos, imageSize), image, QRect(0, 0, image.width(), image.height()));
}

void QuillItem::fileLoaded() {
  update();
}

bool QuillItem::fileError() {
  if (!m_file) {
    return true;
  }

  QuillError err = m_file->error();

  if (err.errorCode() != QuillError::NoError) {
    qWarning() << "Error loading file" << m_file->fileName()
	       << "Code" << err.errorCode() << "Source" << err.errorSource();

    QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection,
			      Q_ARG(QString, err.errorData()));

    m_file->deleteLater(); m_file = 0;

    if (!m_error) {
      m_error = true;

      emit errorChanged();
    }

    return true;
  }

  return false;
}
