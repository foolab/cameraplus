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

#if defined(QT4)
ToolBarLayout::ToolBarLayout(QDeclarativeItem *parent) :
  QDeclarativeItem(parent) {
  setFlag(ItemHasNoContents, false);
#elif defined(QT5)
ToolBarLayout::ToolBarLayout(QQuickItem *parent) :
  QQuickItem(parent) {
#endif

  QObject::connect(this, SIGNAL(visibleChanged()), this, SLOT(itemVisibilityChanged()));
}

ToolBarLayout::~ToolBarLayout() {

}

void ToolBarLayout::componentComplete() {
#if defined(QT4)
  QDeclarativeItem::componentComplete();
#elif defined(QT5)
  QQuickItem::componentComplete();
#endif

  if (isVisible()) {
    reposition();
  }
}

void ToolBarLayout::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
#if defined(QT4)
  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);
#elif defined(QT5)
  QQuickItem::geometryChanged(newGeometry, oldGeometry);
#endif

  if (isComponentComplete() && isVisible()) {
    reposition();
  }
}

void ToolBarLayout::itemVisibilityChanged() {
  if (isComponentComplete() && isVisible()) {
    reposition();
  }
}

void ToolBarLayout::reposition() {
#if defined(QT4)
  QList<QDeclarativeItem *> items;
#elif defined(QT5)
  QList<QQuickItem *> items;
#endif

  qreal totalWidth = 0;

#if defined(QT4)
  foreach (QDeclarativeItem *v, m_items) {
#elif defined(QT5)
  foreach (QQuickItem *v, m_items) {
#endif
    if (v->isVisible()) {
      items << v;
      totalWidth += v->width();
    }
  }

  if (items.size() < 1) {
    return;
  }

  qreal spacing = (width() - totalWidth) / (items.size() - 1);

#if defined(QT4)
  qreal height = QDeclarativeItem::height();
#elif defined(QT5)
  qreal height = QQuickItem::height();
#endif

  totalWidth = 0;
  for (int x = 0; x < items.size(); x++) {
#if defined(QT4)
    QDeclarativeItem *item = items[x];
#elif defined(QT5)
    QQuickItem *item = items[x];
#endif
    item->setY((height - item->height()) / 2);
    item->setX((x * spacing) + totalWidth);

    totalWidth += item->width();
  }
}

void ToolBarLayout::clearItems() {
#if defined(QT4)
  foreach (QDeclarativeItem *v, m_items) {
#elif defined(QT5)
  foreach (QQuickItem *v, m_items) {
#endif
    QObject::disconnect(v, SIGNAL(visibleChanged()), this, SLOT(reposition()));
  }

  m_items.clear();
}

void ToolBarLayout::setItems(const QVariant& children) {
  clearItems();

#if defined(QT4)
  QDeclarativeListReference items = children.value<QDeclarativeListReference>();
#elif defined(QT5)
  QQmlListReference items = children.value<QQmlListReference>();
#endif

  for (int x = 0; x < items.count(); x++) {
    QObject *obj = items.at(x);

#if defined(QT4)
    if (QDeclarativeItem *v = qobject_cast<QDeclarativeItem *>(obj)) {
#elif defined(QT5)
    if (QQuickItem *v = qobject_cast<QQuickItem *>(obj)) {
#endif
      m_items << v;
      QObject::connect(v, SIGNAL(visibleChanged()), this, SLOT(reposition()));
    }
  }

  if (isComponentComplete() && isVisible()) {
    reposition();
  }
}
