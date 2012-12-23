// -*- c++ -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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

#ifndef QT_CAM_MODE_P_H
#define QT_CAM_MODE_P_H

#include <QSize>
#include "qtcamdevice_p.h"
#include "qtcamanalysisbin.h"
#include <gst/pbutils/encoding-profile.h>
#include <gst/pbutils/encoding-target.h>

#ifndef GST_USE_UNSTABLE_API
#define GST_USE_UNSTABLE_API
#endif /* GST_USE_UNSTABLE_API */
#include <gst/interfaces/photography.h>

#define PREVIEW_CAPS "video/x-raw-rgb, width = (int) %1, height = (int) %2, bpp = (int) 32, depth = (int) 24, red_mask = (int) 65280, green_mask = (int) 16711680, blue_mask = (int) -16777216"

class QtCamDevicePrivate;
class PreviewImageHandler;
class DoneHandler;

class QtCamModePrivate {
public:
  QtCamModePrivate(QtCamDevicePrivate *d) : id(-1), dev(d) {}
  virtual ~QtCamModePrivate() {}

  int modeId(const char *mode) {
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

  GstEncodingProfile *loadProfile(const QString& path, const QString& name) {
    GError *error = NULL;

    GstEncodingTarget *target = gst_encoding_target_load_from_file(path.toUtf8().data(), &error);
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

  void resetCaps(const char *property) {
    if (!dev->cameraBin) {
      return;
    }

    g_object_set(dev->cameraBin, property, NULL, NULL);
  }

  bool inNightMode() {
    if (!dev->cameraBin) {
      return false;
    }

    int val = 0;

    g_object_get(dev->videoSource, "scene-mode", &val, NULL);

    return val == GST_PHOTOGRAPHY_SCENE_MODE_NIGHT;
  }

  void setCaps(const char *property, const QSize& resolution, int fps) {
    if (!dev->cameraBin) {
      return;
    }

    if (resolution.width() <= 0 || resolution.height() <= 0) {
      return;
    }

    GstCaps *caps = 0;

    if (fps <= 0) {
      caps = gst_caps_new_simple("video/x-raw-yuv",
				 "width", G_TYPE_INT, resolution.width(),
				 "height", G_TYPE_INT, resolution.height(),
				 NULL);
    }
    else {
      caps = gst_caps_new_simple("video/x-raw-yuv",
				 "width", G_TYPE_INT, resolution.width(),
				 "height", G_TYPE_INT, resolution.height(),
				 "framerate",
				 GST_TYPE_FRACTION_RANGE, fps - 1, 1, fps + 1, 1,
				 NULL);
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
  }

  void setPreviewSize(const QSize& size) {
    if (!dev->cameraBin) {
      return;
    }

    if (size.width() <= 0 && size.height() <= 0) {
      g_object_set(dev->cameraBin, "preview-caps", NULL, "post-previews", FALSE, NULL);
    }
    else {
      QString preview = QString(PREVIEW_CAPS).arg(size.width()).arg(size.height());

      GstCaps *caps = gst_caps_from_string(preview.toAscii());

      g_object_set(dev->cameraBin, "preview-caps", caps, "post-previews", TRUE, NULL);

      gst_caps_unref(caps);
    }
  }

  void setFileName(const QString& file) {
    fileName = file;
  }
  void setTempFileName(const QString& file) {
    tempFileName = file;
  }

  void enableViewfinderFilters() {
    if (dev->viewfinderFilters) {
      dev->viewfinderFilters->setBlocked(false);
    }
  }

  void disableViewfinderFilters() {
    if (dev->viewfinderFilters) {
      dev->viewfinderFilters->setBlocked(true);
    }
  }

  int id;
  QtCamMode *q_ptr;
  QtCamDevicePrivate *dev;
  PreviewImageHandler *previewImageHandler;
  DoneHandler *doneHandler;
  QString fileName;
  QString tempFileName;
};

#endif /* QT_CAM_MODE_P_H */
