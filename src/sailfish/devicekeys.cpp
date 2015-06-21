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

#include "devicekeys.h"
#include <QGuiApplication>
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QWindow>

#define KEY_REPEAT_TIMEOUT 25

DeviceKeys::DeviceKeys(QObject *parent) :
  QObject(parent),
  m_repeating(true),
  m_active(false),
  m_timer(new QTimer(this)),
  m_event(0),
  m_key(0) {

  m_timer->setInterval(KEY_REPEAT_TIMEOUT);
  m_timer->setSingleShot(false);

  QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(repeatEventsIfNeeded()));

  QGuiApplication::instance()->installEventFilter(this);
}

DeviceKeys::~DeviceKeys() {

}

bool DeviceKeys::eventFilter(QObject *obj, QEvent *event) {
  if (!m_active) {
    return QObject::eventFilter(obj, event);
  }

  QWindow *win = dynamic_cast<QWindow *>(obj);
  if (!win || !QGuiApplication::allWindows().contains(win)) {
    return QObject::eventFilter(obj, event);
  }

  int ev = event->type();
  if (ev == QEvent::KeyPress || ev == QEvent::KeyRelease) {
    QKeyEvent *e = dynamic_cast<QKeyEvent *>(event);
    int key = e->key();

    if (e->isAutoRepeat()) {
      return QObject::eventFilter(obj, event);
    }

    if (key == Qt::Key_VolumeUp || key == Qt::Key_VolumeDown) {
      processKeyEvent(key, ev);
    }
  }

  return QObject::eventFilter(obj, event);
}

bool DeviceKeys::isActive() const {
  return m_active;
}

void DeviceKeys::setActive(bool active) {
  if (m_active != active) {
    m_active = active;
    emit activeChanged();
  }
}

bool DeviceKeys::isRepeating() {
  return m_repeating;
}

void DeviceKeys::doRepeat(bool repeat) {
  if (repeat != m_repeating) {
    m_repeating = repeat;
    emit repeatChanged();
  }
}

void DeviceKeys::processKeyEvent(int key, int event) {
  if (event == QEvent::KeyPress && key == Qt::Key_VolumeUp) {
    emit volumeUpPressed();
  } else if (event == QEvent::KeyRelease && key == Qt::Key_VolumeUp) {
    emit volumeUpReleased();
  } else if (event == QEvent::KeyPress && key == Qt::Key_VolumeDown) {
    emit volumeDownPressed();
  } else if (event == QEvent::KeyRelease && key == Qt::Key_VolumeDown) {
    emit volumeDownReleased();
  } else {
    return;
  }

  if (!m_repeating) {
    m_timer->stop();
  }

  if (event == QEvent::KeyRelease) {
    m_timer->stop();
  }

  m_event = event;
  m_key = key;

  m_timer->start();
}

void DeviceKeys::repeatEventsIfNeeded() {
  if (m_event != 0 && m_key != 0 && m_repeating) {
    processKeyEvent(m_key, m_event);
  }
  else {
    m_timer->stop();
  }
}
