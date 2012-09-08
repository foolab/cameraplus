// -*- c++ -*-

#ifndef QT_CAM_MODE_P_H
#define QT_CAM_MODE_P_H

#include <QSize>
#include "qtcamdevice_p.h"
#include <gst/pbutils/encoding-profile.h>
#include <gst/pbutils/encoding-target.h>

#ifndef GST_USE_UNSTABLE_API
#define GST_USE_UNSTABLE_API
#endif /* GST_USE_UNSTABLE_API */
#include <gst/interfaces/photography.h>

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

    GParamSpecEnum *e = (GParamSpecEnum *)pspec;
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

  int id;
  QtCamDevicePrivate *dev;
  PreviewImageHandler *previewImageHandler;
  DoneHandler *doneHandler;
};

#endif /* QT_CAM_MODE_P_H */
