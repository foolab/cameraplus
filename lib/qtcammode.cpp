#include "qtcammode.h"
#include "qtcammode_p.h"
#include "qtcamdevice_p.h"
#include "qtcamdevice.h"
#include <QDebug>
#include "qtcamgstreamermessagehandler.h"
#include "qtcamgstreamermessagelistener.h"
#include <gst/video/video.h>
#include <QImage>

#define PREVIEW_CAPS "video/x-raw-rgb, width = (int) %1, height = (int) %2, bpp = (int) 32, depth = (int) 24, red_mask = (int) 65280, green_mask = (int) 16711680, blue_mask = (int) -16777216"

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
    // TODO: consider a QImage subclass that takes a GstBuffer reference
    QImage cp = image.copy();

    QString fileName = QString::fromUtf8(file);

    QMetaObject::invokeMethod(mode, "previewAvailable",
			      Q_ARG(QImage, cp), Q_ARG(QString, fileName));
  }

  QtCamMode *mode;
};

class DoneHandler : public QtCamGStreamerMessageHandler {
public:
  DoneHandler(QtCamMode *m, const char *done, QObject *parent = 0) :
    QtCamGStreamerMessageHandler(done, parent) {
    mode = m;
  }

  virtual void handleMessage(GstMessage *message) {
    const GstStructure *s = gst_message_get_structure(message);
    if (gst_structure_has_field(s, "filename")) {
      const char *str = gst_structure_get_string(s, "filename");
      if (str) {
	fileName = QString::fromUtf8(str);
      }
    }

    QMetaObject::invokeMethod(mode, "saved", Q_ARG(QString, fileName));
  }

  QString fileName;
  QtCamMode *mode;
};

QtCamMode::QtCamMode(QtCamModePrivate *d, const char *mode, const char *done, QObject *parent) :
  QObject(parent), d_ptr(d) {

  d_ptr->id = d_ptr->modeId(mode);
  d_ptr->previewImageHandler = new PreviewImageHandler(this, this);
  d_ptr->doneHandler = new DoneHandler(this, done, this);
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

  // TODO: check that we can actually do it. Perhaps the pipeline is busy.
  g_object_set(d_ptr->dev->cameraBin, "mode", d_ptr->id, NULL);

  // TODO: is that needed ?
  //  d_ptr->dev->resetCapabilities();

  d_ptr->dev->listener->addHandler(d_ptr->previewImageHandler);
  d_ptr->dev->listener->addHandler(d_ptr->doneHandler);

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
  d_ptr->dev->listener->removeHandler(d_ptr->doneHandler);

  d_ptr->previewImageHandler->setParent(this);
  d_ptr->doneHandler->setParent(this);

  stop();

  d_ptr->dev->active = 0;

  QMetaObject::invokeMethod(d_ptr->dev->q_ptr, "modeChanged");

  emit activeChanged();
}

bool QtCamMode::canCapture() {
  return d_ptr->dev->cameraBin && isActive() && d_ptr->dev->q_ptr->isRunning();
}

bool QtCamMode::isActive() {
  return d_ptr->dev->active == this;
}

void QtCamMode::setPreviewSize(const QSize& size) {
  if (!d_ptr->dev->cameraBin) {
    return;
  }

  if (size.width() <= 0 && size.height() <= 0) {
    g_object_set(d_ptr->dev->cameraBin, "preview-caps", NULL, "post-previews", FALSE, NULL);
  }
  else {
    QString preview = QString(PREVIEW_CAPS).arg(size.width()).arg(size.height());

    GstCaps *caps = gst_caps_from_string(preview.toAscii());

    g_object_set(d_ptr->dev->cameraBin, "preview-caps", caps, "post-previews", TRUE, NULL);

    gst_caps_unref(caps);
  }
}

void QtCamMode::setFileName(const QString& fileName) {
  d_ptr->doneHandler->fileName = fileName;
}

void QtCamMode::setNightMode(bool night) {
  if (d_ptr->night != night) {
    d_ptr->night = night;
    applySettings();

    emit nightModeChanged();
  }
}

bool QtCamMode::inNightMode() const {
  return d_ptr->night;
}
