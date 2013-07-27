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

#include "gridlines.h"
#include <QPainter>

#if defined(QT4)
GridLines::GridLines(QDeclarativeItem *parent) :
  QDeclarativeItem(parent) {
#elif defined(QT5)
GridLines::GridLines(QQuickItem *parent) :
  QQuickPaintedItem(parent) {
#endif

#if defined(QT4)
  setFlag(QGraphicsItem::ItemHasNoContents, false);
#endif
}

GridLines::~GridLines() {

}

#if defined(QT4)
void GridLines::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
			   QWidget* widget) {

  QDeclarativeItem::paint(painter, option, widget);
#elif defined(QT5)
void GridLines::paint(QPainter* painter) {
#endif

  painter->save();
  painter->setPen(QPen(Qt::black, 3));
  painter->drawLines(m_lines, 4);

  painter->setPen(QPen(Qt::white, 1));
  painter->drawLines(m_lines, 4);
  painter->restore();
}

void GridLines::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
#if defined(QT4)
  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);
#elif defined(QT5)
  QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
#endif

  qreal width = newGeometry.width();
  qreal height = newGeometry.height();
  qreal w = newGeometry.width() / 3;
  qreal h = newGeometry.height() / 3;

  m_lines[0] = QLineF(QPointF(0, h), QPointF(width, h));
  m_lines[1] = QLineF(QPointF(0, h * 2), QPointF(width, h * 2));
  m_lines[2] = QLineF(QPointF(w, 0), QPointF(w, height));
  m_lines[3] = QLineF(QPointF(w * 2, 0), QPointF(w * 2, height));
}
