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

#ifndef QUILL_ITEM_H
#define QUILL_ITEM_H

#if defined(QT5)
#include <QQuickItem>
#else
#include <QDeclarativeItem>
#endif
#include <QImage>

class QuillFile;

#if defined(QT5)
class QuillItem : public QQuickItem {
#else
class QuillItem : public QDeclarativeItem {
#endif

  Q_OBJECT

  Q_PROPERTY(bool error READ error NOTIFY errorChanged);
  Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged);
  Q_PROPERTY(QString mimeType READ mimeType WRITE setMimeType NOTIFY mimeTypeChanged);
  Q_PROPERTY(DisplayLevel displayLevel READ displayLevel WRITE setDisplayLevel NOTIFY displayLevelChanged);
  Q_PROPERTY(Priority priority READ priority WRITE setPriority NOTIFY priorityChanged);
  Q_ENUMS(DisplayLevel Priority);

public:
#if defined(QT4)
  QuillItem(QDeclarativeItem *parent = 0);
#else
  QuillItem(QQuickItem *parent = 0);
#endif
  ~QuillItem();

  // TODO: something is really wrong somewhere in Quill.
  // If I replace cropped and full screen levels then loading
  // of full screen previews gets hurt significantly.
  typedef enum {
    DisplayLevelInvalid = -1,
    DisplayLevelFullScreen = 0,
    DisplayLevelCropped = 1,
    DisplayLevelLarge = 2,
  } DisplayLevel;

  typedef enum {
    PriorityLow,
    PriorityHigh,
  } Priority;

#if defined(QT4)
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
#endif

  bool error() const;

  void componentComplete();
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

  QUrl url() const;
  void setUrl(const QUrl& url);

  QString mimeType() const;
  void setMimeType(const QString& mimeType);

  DisplayLevel displayLevel() const;
  void setDisplayLevel(const DisplayLevel& level);

  Priority priority() const;
  void setPriority(const Priority& priority);

#ifdef QT5
protected:
  QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *updatePaintNodeData);
#endif

signals:
  void error(const QString& err);
  void urlChanged();
  void mimeTypeChanged();
  void displayLevelChanged();
  void errorChanged();
  void priorityChanged();

private slots:
  void fileLoaded();
  bool fileError();

private:
  void updateImage();

  QuillFile *m_file;
  bool m_error;
  DisplayLevel m_displayLevel;
  Priority m_priority;
  QUrl m_url;
  QString m_mimeType;
  QImage m_image;
  QRectF m_sourceRect;
  QRectF m_targetRect;
};

#endif /* QUILL_ITEM_H */
