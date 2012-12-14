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

#include "focusrectangle.h"
#include <QPainter>
#include <QDebug>

#define OUTER_WIDTH  4
#define INNER_WIDTH  2
#define OUTER_LENGTH 50
#define INNER_LENGTH 49

FocusRectangle::FocusRectangle(QDeclarativeItem *parent) :
  QDeclarativeItem(parent),
  m_color(Qt::white) {

  setFlag(QGraphicsItem::ItemHasNoContents, false);
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

void FocusRectangle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
			   QWidget* widget) {

  QDeclarativeItem::paint(painter, option, widget);

  painter->save();

  painter->setPen(QPen(QBrush(Qt::black), OUTER_WIDTH));
  painter->drawPath(m_outer);

  painter->setPen(QPen(QBrush(m_color), INNER_WIDTH));
  painter->drawPath(m_inner);

  painter->restore();
}

void FocusRectangle::geometryChanged( const QRectF& newGeometry, const QRectF& oldGeometry) {
  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);

  qreal w = width();
  qreal h = height();

  // Outer:
  // top left:
  m_outer = QPainterPath(QPointF(0, OUTER_LENGTH));
  m_outer.lineTo(0, 0);
  m_outer.lineTo(OUTER_LENGTH, 0);

  // top right:
  m_outer.moveTo(w - OUTER_LENGTH, 0);
  m_outer.lineTo(w, 0);
  m_outer.lineTo(w, OUTER_LENGTH);

  // bottom right:
  m_outer.moveTo(w, h - OUTER_LENGTH);
  m_outer.lineTo(w, h);
  m_outer.lineTo(w - OUTER_LENGTH, h);

  // bottom left:
  m_outer.moveTo(OUTER_LENGTH, h);
  m_outer.lineTo(0, h);
  m_outer.lineTo(0, h - OUTER_LENGTH);

  // Inner:
  // top left:
  m_inner = QPainterPath(QPointF(0, INNER_LENGTH));
  m_inner.lineTo(0, 0);
  m_inner.lineTo(INNER_LENGTH, 0);

  // top right:
  m_inner.moveTo(w - INNER_LENGTH, 0);
  m_inner.lineTo(w, 0);
  m_inner.lineTo(w, INNER_LENGTH);

  // bottom right:
  m_inner.moveTo(w, h - INNER_LENGTH);
  m_inner.lineTo(w, h);
  m_inner.lineTo(w - INNER_LENGTH, h);

  // bottom left:
  m_inner.moveTo(INNER_LENGTH, h);
  m_inner.lineTo(0, h);
  m_inner.lineTo(0, h - INNER_LENGTH);
}
