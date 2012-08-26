// -*- c++ -*-

#ifndef QT_CAM_MODE_P_H
#define QT_CAM_MODE_P_H

#include <QSize>
#include "qtcamdevice_p.h"
#include <gst/pbutils/encoding-profile.h>
#include <gst/pbutils/encoding-target.h>

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

  int id;
  QtCamDevicePrivate *dev;
  PreviewImageHandler *previewImageHandler;
  DoneHandler *doneHandler;
};

#endif /* QT_CAM_MODE_P_H */
