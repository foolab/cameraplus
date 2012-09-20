// -*- c++ -*-

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

#ifndef QUILL_ITEM_H
#define QUILL_ITEM_H

#include <QDeclarativeItem>

class QuillFile;

class QuillItem : public QDeclarativeItem {
  Q_OBJECT

  Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged);
  Q_PROPERTY(QString mimeType READ mimeType WRITE setMimeType NOTIFY mimeTypeChanged);
  Q_PROPERTY(bool error READ error NOTIFY errorChanged);

public:
  QuillItem(QDeclarativeItem *parent = 0);
  ~QuillItem();

  QUrl source() const;
  void setSource(const QUrl& src);

  QString mimeType() const;
  void setMimeType(const QString& mime);

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

  virtual void componentComplete();

  bool error() const;

signals:
  void sourceChanged();
  void error(const QString& err);
  void mimeTypeChanged();
  void errorChanged();

private slots:
  void fileLoaded();
  bool fileError();

private:
  void recreate();

  QuillFile *m_file;
  QUrl m_url;
  QString m_type;
  bool m_error;
};

#endif /* QUILL_ITEM_H */
