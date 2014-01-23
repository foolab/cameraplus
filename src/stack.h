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

#ifndef STACK_H
#define STACK_H

#include <QObject>
#include <QList>

class Stack : public QObject {
  Q_OBJECT

  Q_PROPERTY(int length READ length NOTIFY lengthChanged);

public:
  Stack(QObject *parent = 0);
  ~Stack();

  Q_INVOKABLE QObject *pop();
  Q_INVOKABLE QObject *peek();
  Q_INVOKABLE void push(QObject *comp);

  int length() const;

signals:
  void lengthChanged();

private:
  QList<QObject *> m_list;
};

#endif /* STACK_H */
