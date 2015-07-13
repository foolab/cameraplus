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

#ifndef QT_CAM_VIEWFINDER_FRAME_LISTENER_P_H
#define QT_CAM_VIEWFINDER_FRAME_LISTENER_P_H

#include <QMutex>
#include "qtcamviewfinderframehandler.h"
#include "qtcamviewfinderrenderer.h"
#include "qtcamviewfinderrenderer_p.h"

class QtCamViewfinderFrameListenerPrivate : public QtCamViewfinderRendererBufferInterface {
public:
  QtCamViewfinderFrameListenerPrivate() {
    renderer = 0;
  }

  ~QtCamViewfinderFrameListenerPrivate() {
    qDeleteAll(handlers);
    handlers.clear();
  }

  void setRenderer(QtCamViewfinderRenderer *r) {
    QMutexLocker l(&mutex);
    if (renderer) {
      // disable buffer sending.
      renderer->d_ptr->setInterface(0);
    }

    QtCamViewfinderFrameListenerPrivate::renderer = r;

    if (renderer && handlers.size() > 0) {
      // enable sending buffers
      renderer->d_ptr->setInterface(this);
    }
  }

  void addHandler(QtCamViewfinderFrameHandler *handler) {
    QMutexLocker l(&mutex);
    if (handlers.indexOf(handler) == -1) {
      handlers << handler;
    }

    if (renderer && handlers.size() == 1) {
      // enable sending buffers
      renderer->d_ptr->setInterface(this);
    }
  }

  void removeHandler(QtCamViewfinderFrameHandler *handler) {
    QMutexLocker l(&mutex);

    int index = handlers.indexOf(handler);
    if (index != -1) {
      handlers.takeAt(index);
    }

    if (renderer && handlers.size() == 0) {
      // disable sending buffers
      renderer->d_ptr->setInterface(0);
    }
  }

private:
  virtual void handleData(unsigned char *data,
			  const QSize& size,
			  const QtCamViewfinderFrame::Format& format) {
    QMutexLocker l(&mutex);
    // TODO: harmattan specific
    QByteArray d = QByteArray::fromRawData((const char *)data, size.width() * size.height() * 2);
    QtCamViewfinderFrame frame(d, size, format);

    foreach (QtCamViewfinderFrameHandler *handler, handlers) {
      handler->handleFrame(&frame);
    }
  }

  QtCamViewfinderRenderer *renderer;
  QMutex mutex;
  QList<QtCamViewfinderFrameHandler *> handlers;
};

#endif /* QT_CAM_VIEWFINDER_FRAME_LISTENER_P_H */
