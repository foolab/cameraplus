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

#ifndef TOOL_BAR_LAYOUT_H
#define TOOL_BAR_LAYOUT_H

#if defined(QT4)
#include <QDeclarativeItem>
#elif defined(QT5)
#include <QQuickItem>
#endif

#if defined(QT4)
class ToolBarLayout : public QDeclarativeItem {
#elif defined(QT5)
class ToolBarLayout : public QQuickItem {
#endif

  Q_OBJECT

public:
#if defined(QT4)
  ToolBarLayout(QDeclarativeItem *parent = 0);
#elif defined(QT5)
  ToolBarLayout(QQuickItem *parent = 0);
#endif
  ~ToolBarLayout();

  void componentComplete();

public slots:
  void clearItems();
  void setItems(const QVariant& children);

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

private slots:
  void reposition();
  void itemVisibilityChanged();

private:
#if defined(QT4)
  QList<QDeclarativeItem *> m_items;
#elif defined(QT5)
  QList<QQuickItem *> m_items;
#endif
};

#endif /* TOOL_BAR_LAYOUT_H */
