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

#ifndef GRID_LINES_H
#define GRID_LINES_H

#if defined(QT4)
#include <QDeclarativeItem>
#elif defined(QT5)
#include <QQuickPaintedItem>
#endif
#include <QLineF>

#if defined(QT4)
class GridLines : public QDeclarativeItem {
#elif defined(QT5)
class GridLines : public QQuickPaintedItem {
#endif

  Q_OBJECT

public:
#if defined(QT4)
  GridLines(QDeclarativeItem *parent = 0);
#elif defined(QT5)
  GridLines(QQuickItem *parent = 0);
#endif
  ~GridLines();

#if defined(QT4)
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
#elif defined(QT5)
  void paint(QPainter* painter);
#endif

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

private:
  QLineF m_lines[4];
};

#endif /* GRID_LINES_H */
