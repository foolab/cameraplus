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

#include "qtcammode.h"
#include "qtcammode_p.h"
#include "qtcamdevice_p.h"
#include "qtcamdevice.h"
#include <QDebug>
#include "qtcamgstreamermessagelistener.h"
#include <gst/video/video.h>
#include <QImage>
#include <QFile>

class PreviewImageHandler : public QtCamGStreamerMessageHandler {
public:
  PreviewImageHandler(QtCamMode *m, QObject *parent = 0) :
    QtCamGStreamerMessageHandler("preview-image", parent) {
    mode = m;
  }

  virtual void handleMessage(GstMessage *message) {
    const GstStructure *s = gst_message_get_structure(message);
    if (!s) {
      return;
    }

    const char *file = gst_structure_get_string(s, "location");
    if (!file) {
      return;
    }

    const GValue *val = gst_structure_get_value(s, "buffer");
    if (!val) {
      return;
    }

    GstBuffer *buffer = gst_value_get_buffer(val);
    if (!buffer) {
      return;
    }

    int width, height;
    GstVideoFormat fmt;
    if (!gst_video_format_parse_caps(buffer->caps, &fmt, &width, &height)) {
      return;
    }

    if (fmt !=  GST_VIDEO_FORMAT_BGRx || width <= 0 || height <= 0) {
      return;
    }

    QImage image(buffer->data, width, height, QImage::Format_RGB32);

    // We need to copy because GStreamer will free the buffer after we return
    // and since QImage doesn't copythe data by default we will end up with garbage.
    // There is no way to subclass QImage to prevent copying :|
    QImage cp = image.copy();

    QString fileName = QString::fromUtf8(file);

    QMetaObject::invokeMethod(mode, "previewAvailable",
			      Q_ARG(QImage, cp), Q_ARG(QString, fileName));
  }

  QtCamMode *mode;
};

QtCamMode::QtCamMode(QtCamModePrivate *d, const char *mode, QObject *parent) :
  QObject(parent), d_ptr(d) {

  d_ptr->q_ptr = this;
  d_ptr->id = d_ptr->modeId(mode);
  d_ptr->previewImageHandler = new PreviewImageHandler(this, this);
  d_ptr->doneHandler = 0;
}

QtCamMode::~QtCamMode() {
  delete d_ptr; d_ptr = 0;
}

void QtCamMode::activate() {
  if (!d_ptr->dev->cameraBin) {
    return;
  }

  if (d_ptr->dev->active == this) {
    return;
  }

  if (d_ptr->dev->active) {
    d_ptr->dev->active->deactivate();
  }

  d_ptr->dev->active = this;

  g_object_set(d_ptr->dev->cameraBin, "mode", d_ptr->id, NULL);

  d_ptr->dev->listener->addHandler(d_ptr->previewImageHandler);

  // This has to be sync. VideoDoneHandler will lock a mutex that is already
  // locked from the main thread.
  d_ptr->dev->listener->addSyncHandler(d_ptr->doneHandler);

  start();

  applySettings();

  QMetaObject::invokeMethod(d_ptr->dev->q_ptr, "modeChanged");

  emit activeChanged();
}

void QtCamMode::deactivate() {
  if (d_ptr->dev->active != this) {
    return;
  }

  d_ptr->dev->listener->removeHandler(d_ptr->previewImageHandler);
  d_ptr->dev->listener->removeSyncHandler(d_ptr->doneHandler);

  d_ptr->previewImageHandler->setParent(this);
  d_ptr->doneHandler->setParent(this);

  stop();

  d_ptr->dev->active = 0;

  QMetaObject::invokeMethod(d_ptr->dev->q_ptr, "modeChanged");

  emit activeChanged();
}

bool QtCamMode::canCapture() {
  return d_ptr->dev->cameraBin && isActive() && d_ptr->dev->q_ptr->isRunning() &&
    !d_ptr->dev->error;
}

bool QtCamMode::isActive() {
  return d_ptr->dev->active == this;
}

QtCamDevice *QtCamMode::device() const {
  return d_ptr->dev->q_ptr;
}
