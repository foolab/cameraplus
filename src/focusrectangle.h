// -*- c++ -*-

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

#ifndef FOCUS_RECTANGLE_H
#define FOCUS_RECTANGLE_H

#if defined(QT4)
#include <QDeclarativeItem>
#elif defined(QT5)
#include <QQuickPaintedItem>
#endif
#include <QPainterPath>

#if defined(QT4)
class FocusRectangle : public QDeclarativeItem {
#elif defined(QT5)
class FocusRectangle : public QQuickPaintedItem {
#endif

  Q_OBJECT

  Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged);

public:
#if defined(QT4)
  FocusRectangle(QDeclarativeItem *parent = 0);
#elif defined(QT5)
  FocusRectangle(QQuickItem *parent = 0);
#endif

  ~FocusRectangle();

  QColor color() const;
  void setColor(const QColor& color);

#if defined(QT4)
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
#elif defined(QT5)
  void paint(QPainter* painter);
#endif

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

signals:
  void colorChanged();

private:
  QColor m_color;

  QPainterPath m_path;
};

#endif /* FOCUS_RECTANGLE_H */
