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
#if defined(QT4)
#include <QPainter>
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#include <QQuickWindow>
#include <QSGNode>
#include <QSGSimpleTextureNode>
#endif

#if defined(QT5)
QuillItem::QuillItem(QQuickItem *parent) :
  QQuickItem(parent),
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
#else
  setFlag(QQuickItem::ItemHasContents, true);
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
  QQuickItem::componentComplete();
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

  updateImage(false);
}

void QuillItem::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
#if defined(QT4)
  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);
#elif defined(QT5)
  QQuickItem::geometryChanged(newGeometry, oldGeometry);
#endif

  updateImage(false);
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

  updateImage(true);
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

  painter->drawImage(m_targetRect, m_image, m_sourceRect);
}
#endif

void QuillItem::fileLoaded() {
  updateImage(true);
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

void QuillItem::updateImage(bool callUpdate) {
  if (m_file) {
    QImage image = m_file->image(m_displayLevel);
    if (!image.isNull()) {
      m_image = image;
    }

    QRectF br = boundingRect();
    QRectF rect = QRectF(br.x(), br.y(), br.width() * scale(), br.height() * scale());
    QRectF target = rect;
    QSizeF size = m_image.size();
    size.scale(rect.size(), Qt::KeepAspectRatio);
    target.setSize(size);
    QPointF pos(qAbs((rect.width() - target.width()) / 2),
		qAbs((rect.height() - target.height()) / 2));
    target.moveTo(pos);
    m_targetRect = target;

    if (callUpdate) {
      update();
    }
  }
}

#ifdef QT5
class QuillNode : public QSGNode {
public:
  QuillNode(QQuickWindow *window);
  ~QuillNode();

  void setImage(const QImage& image, const QRectF& target);

private:
  QQuickWindow *m_window;
  QSGTexture *m_texture;
  QSGSimpleTextureNode *m_textureNode;
};

QuillNode::QuillNode(QQuickWindow *window) :
  m_window(window),
  m_texture(0),
  m_textureNode(0) {

}

QuillNode::~QuillNode() {
  if (m_texture) {
    delete m_texture;
  }

  if (m_textureNode) {
    removeChildNode(m_textureNode);
    delete m_textureNode;
  }
}

void QuillNode::setImage(const QImage& image, const QRectF& target) {
  if (m_texture) {
    delete m_texture;
  }

  m_texture = m_window->createTextureFromImage(image);

  if (m_textureNode) {
    removeChildNode(m_textureNode);
    delete m_textureNode;
  }

  m_textureNode = new QSGSimpleTextureNode;
  m_textureNode->setTexture(m_texture);
  m_textureNode->setRect(target);
  m_textureNode->setFiltering(QSGTexture::Linear);
  appendChildNode(m_textureNode);
}

QSGNode *QuillItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *updatePaintNodeData) {
  Q_UNUSED(updatePaintNodeData);

  QuillNode *node = dynamic_cast<QuillNode *>(oldNode);

  if (width() <= 0 || height() <= 0) {
    delete node;
    return 0;
  }

  if (!node) {
    node = new QuillNode(window());
  }

  node->setImage(m_image, m_targetRect);

  return node;
}
#endif
