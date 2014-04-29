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
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

#if defined(QT5)
QuillItem::QuillItem(QQuickItem *parent) :
  QQuickPaintedItem(parent),
#else
QuillItem::QuillItem(QDeclarativeItem *parent) :
  QDeclarativeItem(parent),
#endif
  m_file(0),
  m_error(false),
  m_displayLevel(DisplayLevelInvalid),
  m_priority(PriorityLow) {

#if defined(QT4)
  setFlag(QGraphicsItem::ItemHasNoContents, false);
#endif

}

QuillItem::~QuillItem() {
  if (m_file) {
    delete m_file;
    m_file = 0;
  }
}

void QuillItem::componentComplete() {
#if defined(QT4)
  QDeclarativeItem::componentComplete();
#elif defined(QT5)
  QQuickPaintedItem::componentComplete();
#endif

  m_file = new QuillFile(m_url.toLocalFile(), m_mimeType);
  m_file->setPriority(m_priority == PriorityHigh ?
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

  if (m_displayLevel == DisplayLevelInvalid) {
    qmlInfo(this) << "display level not set";
    return;
  }

  m_file->setDisplayLevel(m_displayLevel);

  fileError();
}

QUrl QuillItem::url() const {
  return m_url;
}

void QuillItem::setUrl(const QUrl& url) {
  if (url == m_url) {
    return;
  }

  if (m_file) {
    qmlInfo(this) << "url cannot be set after initialization";
    return;
  }

  m_url = url;

  emit urlChanged();
}

QString QuillItem::mimeType() const {
  return m_mimeType;
}

void QuillItem::setMimeType(const QString& mimeType) {
  if (mimeType == m_mimeType) {
    return;
  }

  if (m_file) {
    qmlInfo(this) << "mime type cannot be set after initialization";
    return;
  }

  m_mimeType = mimeType;

  emit mimeTypeChanged();
}

QuillItem::DisplayLevel QuillItem::displayLevel() const {
  return m_displayLevel;
}

void QuillItem::setDisplayLevel(const QuillItem::DisplayLevel& level) {
  if (m_displayLevel == level) {
    return;
  }

  m_displayLevel = level;

  if (m_file) {
    m_file->setDisplayLevel(level);
    fileError();
  }

  emit displayLevelChanged();
}

QuillItem::Priority QuillItem::priority() const {
  return m_priority;
}

void QuillItem::setPriority(const QuillItem::Priority& priority) {
  if (priority == m_priority) {
    return;
  }

  m_priority = priority;

  if (m_file) {
    m_file->setPriority(m_priority == PriorityHigh ?
			QuillFile::Priority_High : QuillFile::Priority_Low);
    fileError();
  }

  emit priorityChanged();
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
    if (m_displayLevel == QuillItem::DisplayLevelLarge
	&& m_file->hasThumbnail(QuillItem::DisplayLevelFullScreen)) {
      image = m_file->image(QuillItem::DisplayLevelFullScreen);
    } else {
      return;
    }
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
