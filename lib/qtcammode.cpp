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

#include "qtcammode.h"
#include "qtcammode_p.h"
#include "qtcamdevice_p.h"
#include "qtcamdevice.h"
#include <QDebug>
#include "qtcamgstmessagelistener.h"
#include <gst/video/video.h>
#include <gst/pbutils/encoding-profile.h>
#include <gst/pbutils/encoding-target.h>
#include <QImage>
#include <QFile>
#ifndef GST_USE_UNSTABLE_API
#define GST_USE_UNSTABLE_API
#endif /* GST_USE_UNSTABLE_API */
#include <gst/interfaces/photography.h>

#if GST_CHECK_VERSION(1,0,0)
#define PREVIEW_CAPS "video/x-raw, format = (string)BGRx, width = (int) %1, height = (int) %2"
#else
#define PREVIEW_CAPS "video/x-raw-rgb, width = (int) %1, height = (int) %2, bpp = (int) 32, depth = (int) 24, red_mask = (int) 65280, green_mask = (int) 16711680, blue_mask = (int) -16777216"
#endif

#define CAPS_NO_FPS "%s, width=(int)%d,height=(int)%d"
#define CAPS_FPS "%s, width=(int)%d,height=(int)%d,framerate=(fraction)[%d/%d,%d/%d]"

class PreviewImageHandler : public QtCamGstMessageHandler {
public:
  PreviewImageHandler(QtCamMode *m, QObject *parent = 0) :
    QtCamGstMessageHandler("preview-image", parent) {
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

#warning FIX ME
#if !GST_CHECK_VERSION(1,0,0)
    if (!gst_video_format_parse_caps(buffer->caps, &fmt, &width, &height)) {
      return;
    }
#endif

    if (fmt !=  GST_VIDEO_FORMAT_BGRx || width <= 0 || height <= 0) {
      return;
    }

#warning FIX ME
#if GST_CHECK_VERSION(1,0,0)
    QImage image;
#else
    QImage image(buffer->data, width, height, QImage::Format_RGB32);
#endif

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

void QtCamMode::disablePreview() {
  d_ptr->setPreviewSize(QSize());
}

void QtCamMode::setEncodingProfile(GstEncodingProfile *profile,
				   const QLatin1String& propertyName) {
  if (d_ptr->dev->cameraBin) {
    g_object_set(d_ptr->dev->cameraBin, propertyName.latin1(), profile, NULL);
  }

  gst_encoding_profile_unref(profile);
}

// Private implementation:
QtCamModePrivate::QtCamModePrivate(QtCamDevicePrivate *d) :
  id(-1), dev(d) {

}

QtCamModePrivate::~QtCamModePrivate() {

}

void QtCamModePrivate::init(DoneHandler *handler) {
  doneHandler = handler;
}

int QtCamModePrivate::modeId(const char *mode) {
  if (!dev->cameraBin) {
    return -1;
  }

  GParamSpec *pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(dev->cameraBin),
						   "mode");
  if (!pspec) {
    return -1;
  }

  if (!G_IS_PARAM_SPEC_ENUM(pspec)) {
    return -1;
  }

  GParamSpecEnum *e = G_PARAM_SPEC_ENUM(pspec);
  GEnumClass *klass = e->enum_class;

  for (unsigned x = 0; x < klass->n_values; x++) {
    if (QLatin1String(mode) == QLatin1String(klass->values[x].value_nick)) {
      return klass->values[x].value;
    }
  }

  return -1;
}

GstEncodingProfile *QtCamModePrivate::loadProfile(const QString& path, const QString& name) {
  GError *error = NULL;
  QString targetPath;
  QFileInfo info(path);
  if (!info.isAbsolute()) {
    targetPath = dev->conf->lookUp(path);
  } else {
    targetPath = info.filePath();
  }

  GstEncodingTarget *target = gst_encoding_target_load_from_file(targetPath.toUtf8().constData(),
								 &error);
  if (!target) {
    qCritical() << "Failed to load encoding target from" << path << error->message;
    g_error_free(error);
    return 0;
  }

  GstEncodingProfile *profile = gst_encoding_target_get_profile(target, name.toUtf8().data());
  if (!profile) {
    qCritical() << "Failed to load encoding profile from" << path;
    gst_encoding_target_unref(target);
    return 0;
  }

  gst_encoding_target_unref(target);

  return profile;
}

void QtCamModePrivate::resetCaps(const char *property) {
  if (!dev->cameraBin) {
    return;
  }

  g_object_set(dev->cameraBin, property, NULL, NULL);
}

bool QtCamModePrivate::inNightMode() {
  if (!dev->cameraBin) {
    return false;
  }

  int val = 0;

  g_object_get(dev->videoSource, "scene-mode", &val, NULL);

  return val == GST_PHOTOGRAPHY_SCENE_MODE_NIGHT;
}

void QtCamModePrivate::setCaps(const char *property, const QSize& resolution, int fps) {
  QString mediaType = dev->conf->mediaType(property);
  QByteArray arr = mediaType.toLatin1();
  const gchar *media = arr.isEmpty() ? NULL : arr.constData();

  QString format = dev->conf->mediaFourcc(property);
  QByteArray mediaArr = format.toLatin1();

#if GST_CHECK_VERSION(1,0,0)
  const gchar *fourcc = mediaArr.isEmpty() ? NULL : mediaArr.constData();
#else
  unsigned long fourcc = GST_STR_FOURCC(mediaArr.constData());
#endif

  if (!dev->cameraBin) {
    return;
  }

  if (resolution.width() <= 0 || resolution.height() <= 0) {
    return;
  }

  GstCaps *caps = 0;
  gchar *tpl = 0;

  if (fps <= 0) {
    tpl = g_strdup_printf (CAPS_NO_FPS, media,
			   resolution.width(),
			   resolution.height());
  } else {
    tpl = g_strdup_printf (CAPS_FPS, media,
			   resolution.width(),
			   resolution.height(),
			   fps - 1, 1, fps + 1, 1);
  }

  caps = gst_caps_from_string (tpl);
  g_free (tpl);

  if (fourcc) {
#if GST_CHECK_VERSION(1,0,0)
    gst_caps_set_simple (caps, "format", G_TYPE_STRING, fourcc, NULL);
#else
    gst_caps_set_simple (caps, "format", GST_TYPE_FOURCC, fourcc, NULL);
#endif
  }

  GstCaps *old = 0;

  g_object_get(dev->cameraBin, property, &old, NULL);

  if (gst_caps_is_equal(caps, old)) {
    gst_caps_unref(old);
    gst_caps_unref(caps);

    return;
  }

  g_object_set(dev->cameraBin, property, caps, NULL);

  if (old) {
    gst_caps_unref(old);
  }

  gst_caps_unref(caps);
}

void QtCamModePrivate::setPreviewSize(const QSize& size) {
  if (!dev->cameraBin) {
    return;
  }

  if (size.width() <= 0 && size.height() <= 0) {
    g_object_set(dev->cameraBin, "preview-caps", NULL, "post-previews", FALSE, NULL);
  } else {
    QString preview = QString(PREVIEW_CAPS).arg(size.width()).arg(size.height());

    GstCaps *caps = gst_caps_from_string(preview.toLatin1());

    if (!dev->conf->isPreviewSupported()) {
      qWarning() << "Cannot set preview caps. Preview not supported";
      gst_caps_unref(caps);
      return;
    }

    g_object_set(dev->cameraBin, "preview-caps", caps, "post-previews", TRUE, NULL);

    gst_caps_unref(caps);
  }
}

void QtCamModePrivate::setFileName(const QString& file) {
  fileName = file;
}

void QtCamModePrivate::setTempFileName(const QString& file) {
  tempFileName = file;
}

void QtCamModePrivate::enableViewfinderFilters() {
  if (dev->viewfinderFilters) {
    if (!dev->conf->viewfinderFiltersUseAnalysisBin()) {
      qWarning() << "Cannot enable viewfinder filters without a controlling bin";
      return;
    }

    g_object_set(dev->viewfinderFilters, "bypass", FALSE, NULL);
  }
}

void QtCamModePrivate::disableViewfinderFilters() {
  if (dev->viewfinderFilters) {
    if (!dev->conf->viewfinderFiltersUseAnalysisBin()) {
      qWarning() << "Cannot disable viewfinder filters without a controlling bin";
      return;
    }

    g_object_set(dev->viewfinderFilters, "bypass", TRUE, NULL);
  }
}
