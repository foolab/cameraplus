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

#include "focusrectangle.h"
#include <QPainter>
#include <QDebug>

#define WIDTH  4
#define LENGTH 30

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

  painter->setPen(QPen(QBrush(Qt::black), WIDTH));
  painter->drawPath(m_path);

  painter->setPen(QPen(QBrush(m_color), WIDTH - 2));
  painter->drawPath(m_path);

  painter->restore();
}

void FocusRectangle::geometryChanged( const QRectF& newGeometry, const QRectF& oldGeometry) {
  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);

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
