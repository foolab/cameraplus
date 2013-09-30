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

#include <QDeclarativeItem>

class QuillFile;

class QuillItem : public QDeclarativeItem {
  Q_OBJECT

  Q_PROPERTY(bool error READ error NOTIFY errorChanged);

public:
  QuillItem(QDeclarativeItem *parent = 0);
  ~QuillItem();

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

  bool error() const;

public slots:
  void initialize(const QUrl& url, const QString& mimeType, int displayLevel);

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
  void updateImage();

  QuillFile *m_file;
  bool m_error;
  int m_displayLevel;
};

#endif /* QUILL_ITEM_H */
