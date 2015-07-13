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

#include "qtcamviewfinderframelistener.h"
#include "qtcamviewfinderframelistener_p.h"

QtCamViewfinderFrameListener::QtCamViewfinderFrameListener(QObject *parent) :
  QObject(parent),
  d_ptr(new QtCamViewfinderFrameListenerPrivate) {

}

QtCamViewfinderFrameListener::~QtCamViewfinderFrameListener() {
  delete d_ptr; d_ptr = 0;
}

void QtCamViewfinderFrameListener::addHandler(QtCamViewfinderFrameHandler *handler) {
  d_ptr->addHandler(handler);
}

void QtCamViewfinderFrameListener::removeHandler(QtCamViewfinderFrameHandler *handler) {
  d_ptr->removeHandler(handler);
}
