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

#include "viewfinderbufferhandler.h"
#include "qtcamdevice.h"
#include "qtcamviewfinderbufferlistener.h"
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

ViewfinderBufferHandler::ViewfinderBufferHandler(QObject *parent) :
  ViewfinderHandler("handleSample(const QtCamGstSample *)", parent) {

}

ViewfinderBufferHandler::~ViewfinderBufferHandler() {
  deviceAboutToChange();
}

void ViewfinderBufferHandler::registerHandler(QtCamDevice *dev) {
  dev->bufferListener()->addHandler(this);
}

void ViewfinderBufferHandler::unregisterHandler(QtCamDevice *dev) {
  dev->bufferListener()->removeHandler(this);
}

void ViewfinderBufferHandler::handleSample(const QtCamGstSample *sample) {
  QMutexLocker l(&m_mutex);

  if (!m_method.enclosingMetaObject()) {
    return;
  }

  if (!m_method.invoke(m_handler, Qt::DirectConnection,
		       Q_ARG(const QtCamGstSample *, sample))) {
    qmlInfo(this) << "Failed to invoke handler";
  }
}
