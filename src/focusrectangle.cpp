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

#include "focusrectangle.h"
#include <QPainter>

#define WIDTH  4
#define LENGTH 30

#if defined(QT4)
FocusRectangle::FocusRectangle(QDeclarativeItem *parent) :
  QDeclarativeItem(parent),
#elif defined(QT5)
FocusRectangle::FocusRectangle(QQuickItem *parent) :
  QQuickPaintedItem(parent),
#endif
  m_color(Qt::white) {

#if defined(QT4)
  setFlag(QGraphicsItem::ItemHasNoContents, false);
#endif
}

FocusRectangle::~FocusRectangle() {

}

QColor FocusRectangle::color() const {
  return m_color;
}

void FocusRectangle::setColor(const QColor& color) {
  if (color != m_color) {
    m_color = color;
    emit colorChanged();

    update();
  }
}

#if defined(QT4)
void FocusRectangle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
			   QWidget* widget) {

  QDeclarativeItem::paint(painter, option, widget);
#elif defined(QT5)
void FocusRectangle::paint(QPainter* painter) {
#endif

  painter->save();

  painter->setPen(QPen(QBrush(Qt::black), WIDTH));
  painter->drawPath(m_path);

  painter->setPen(QPen(QBrush(m_color), WIDTH - 2));
  painter->drawPath(m_path);

  painter->restore();
}

void FocusRectangle::geometryChanged( const QRectF& newGeometry, const QRectF& oldGeometry) {
#if defined(QT4)
  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);
#elif defined(QT5)
  QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
#endif

  qreal w = width();
  qreal h = height();

  qreal offset = WIDTH / 2;

  // top left:
  m_path = QPainterPath(QPointF(offset, LENGTH - offset));
  m_path.lineTo(offset, offset);
  m_path.lineTo(LENGTH - offset, offset);

  // top right:
  m_path.moveTo(w - LENGTH + offset, offset);
  m_path.lineTo(w - offset, offset);
  m_path.lineTo(w - offset, LENGTH - offset);

  // bottom right:
  m_path.moveTo(w - offset, h - LENGTH + offset);
  m_path.lineTo(w - offset, h - offset);
  m_path.lineTo(w - LENGTH + offset, h - offset);

  // bottom left:
  m_path.moveTo(LENGTH - offset, h - offset);
  m_path.lineTo(offset, h - offset);
  m_path.lineTo(offset, h - LENGTH + offset);
}
