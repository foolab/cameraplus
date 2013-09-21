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

#include "toolbarlayout.h"

ToolBarLayout::ToolBarLayout(QDeclarativeItem *parent) :
  QDeclarativeItem(parent) {

  setFlag(ItemHasNoContents, false);
}

ToolBarLayout::~ToolBarLayout() {

}

void ToolBarLayout::componentComplete() {
  QDeclarativeItem::componentComplete();

  if (isVisible()) {
    reposition();
  }
}

void ToolBarLayout::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);

  if (isComponentComplete() && isVisible()) {
    reposition();
  }
}

QVariant ToolBarLayout::itemChange(GraphicsItemChange change, const QVariant& value) {
  QVariant result = QDeclarativeItem::itemChange(change, value);
  if (!isComponentComplete()) {
    return result;
  }

  if (change == QGraphicsItem::ItemVisibleHasChanged && isVisible()) {
    reposition();
  }

  return result;
}

void ToolBarLayout::reposition() {
  QList<QDeclarativeItem *> items;

  qreal totalWidth = 0;

  foreach (QDeclarativeItem *v, m_items) {
    if (v->isVisible()) {
      items << v;
      totalWidth += v->width();
    }
  }

  if (items.size() < 1) {
    return;
  }

  qreal spacing = (width() - totalWidth) / (items.size() - 1);
  qreal height = QDeclarativeItem::height();
  totalWidth = 0;
  for (int x = 0; x < items.size(); x++) {
    QDeclarativeItem *item = items[x];
    item->setY((height - item->height()) / 2);
    item->setX((x * spacing) + totalWidth);

    totalWidth += item->width();
  }
}

void ToolBarLayout::clearItems() {
  foreach (QDeclarativeItem *v, m_items) {
    QObject::disconnect(v, SIGNAL(visibleChanged()), this, SLOT(reposition()));
  }

  m_items.clear();
}

void ToolBarLayout::setItems(const QVariant& children) {
  clearItems();

  QDeclarativeListReference items = children.value<QDeclarativeListReference>();

  for (int x = 0; x < items.count(); x++) {
    QObject *obj = items.at(x);
    if (QDeclarativeItem *v = qobject_cast<QDeclarativeItem *>(obj)) {
      m_items << v;
      QObject::connect(v, SIGNAL(visibleChanged()), this, SLOT(reposition()));
    }
  }

  if (isComponentComplete() && isVisible()) {
    reposition();
  }
}
