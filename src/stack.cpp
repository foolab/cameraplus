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

#include "stack.h"
Stack::Stack(QObject *parent) :
  QObject(parent) {

}

Stack::~Stack() {

}

QObject *Stack::pop() {
  QObject *ret = m_list.isEmpty() ? 0 : m_list.takeAt(m_list.size() - 1);

  if (ret) {
    emit lengthChanged();
  }

  return ret;
}

void Stack::push(QObject *comp) {
  m_list << comp;
  emit lengthChanged();
}

QObject *Stack::peek() {
  QObject *ret = m_list.isEmpty() ? 0 : m_list.last();

  return ret;
}

int Stack::length() const {
  return m_list.size();
}
