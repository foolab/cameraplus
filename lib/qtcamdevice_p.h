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

#ifndef QT_CAM_DEVICE_P_H
#define QT_CAM_DEVICE_P_H

#include <QDebug>
#include <gst/gst.h>
#include "qtcamconfig.h"
#include "qtcamviewfinder.h"
#include "qtcamdevice.h"
#include "qtcammode.h"
#include "qtcamviewfinderbufferlistener.h"
#include "qtcamviewfinderbufferlistener_p.h"
#include "qtcamresolution.h"
#include "qtcamutils.h"

class QtCamGstMessageListener;
class QtCamMode;
class QtCamImageMode;
class QtCamVideoMode;
class QtCamPropertySetter;
class QtCamAnalysisBin;
class QtCamViewfinderFrameListener;

class QtCamDevicePrivate {
public:
  QtCamDevicePrivate() :
    cameraBin(0),
    videoSource(0),
    wrapperVideoSource(0),
    sink(0),
    image(0),
    video(0),
    active(0),
    viewfinder(0),
    conf(0),
    error(false),
    notifications(0),
    viewfinderFilters(0),
    imageSettings(0),
    videoSettings(0) {

  }

  GstElement *createAndAddElement(const QString& elementName, const char *prop, const char *name) {
    GstElement *elem = gst_element_factory_make(elementName.toLatin1(), name);
    if (!elem) {
      qWarning() << "Failed to create" << elementName;
      return 0;
    }

    g_object_set(cameraBin, prop, elem, NULL);

    return elem;
  }

  void createAndAddVideoSourceAndWrapper() {
    videoSource = gst_element_factory_make(conf->videoSource().toUtf8().constData(),
					   "QtCameraVideoSrc");
    wrapperVideoSource = gst_element_factory_make(conf->wrapperVideoSource().toUtf8().constData(),
						  "QCameraWrapperVideoSrc");

    if (wrapperVideoSource && videoSource) {
      g_object_set(wrapperVideoSource, conf->wrapperVideoSourceProperty().toUtf8().constData(),
		   videoSource, NULL);
      g_object_set(cameraBin, "camera-source", wrapperVideoSource, NULL);
    }
    else if (wrapperVideoSource) {
      qWarning() << "Failed to create video source";
      g_object_set(cameraBin, "camera-source", wrapperVideoSource, NULL);
    }
    else if (videoSource) {
      qWarning() << "Failed to create wrapper source";
      g_object_set(cameraBin, "camera-source", videoSource, NULL);
    }
    else {
      qWarning() << "Failed to create both video and wrapper sources";
    }
  }

  void createAndAddVideoSource() {
    videoSource = gst_element_factory_make(conf->videoSource().toUtf8().constData(),
					   "QtCameraVideoSrc");
    if (!videoSource) {
      qCritical() << "Failed to create video source";
    }
    else {
      g_object_set(cameraBin, "camera-source", videoSource, NULL);
    }
  }

  void setDevicePoperty() {
    if (videoSource) {
      if (conf->deviceScannerType() == SCANNER_TYPE_ENUM) {
	int dev = id.toInt();
	g_object_set(videoSource, conf->deviceScannerProperty().toLatin1().constData(), dev, NULL);
      }
      else {
	QString dev = id.toString();
	g_object_set(videoSource, conf->deviceScannerProperty().toLatin1().constData(),
		     dev.toLatin1().constData(), NULL);
      }
    }
  }

  bool setViewfinderSink() {
    if (sink) {
      return true;
    }

    sink = viewfinder->sinkElement();
    if (!sink) {
      qCritical() << "Failed to create GStreamer sink element";
      bufferListener->d_ptr->setSink(0);
      return false;
    }

    g_object_set(cameraBin, "viewfinder-sink", sink, NULL);

    bufferListener->d_ptr->setSink(sink);

    return true;
  }

  GstElement *createAndAddFilters(const char *name, const char *prop,
				  const QStringList& elements, bool useAnalysisBin) {

    GstElement *bin = gst_bin_new (useAnalysisBin ? NULL : name);
    GstElement *first = 0;
    GstElement *last = 0;

    foreach (const QString& elem, elements) {
      GstElement *e = gst_element_factory_make (elem.toUtf8().constData(), NULL);
      if (!e) {
	qWarning() << "Failed to create element" << elem;
	continue;
      }

      if (!first) {
	first = e;
      }

      gst_bin_add (GST_BIN (bin), e);
      if (last) {
	gst_element_link (last, e);
      }

      last = e;
    }

    GstPad *pad = gst_element_get_static_pad(first, "sink");
    gst_element_add_pad(bin, gst_ghost_pad_new("sink", pad));
    gst_object_unref(GST_OBJECT(pad));

    pad = gst_element_get_static_pad(last, "src");
    gst_element_add_pad(bin, gst_ghost_pad_new("src", pad));
    gst_object_unref(GST_OBJECT(pad));

    if (!useAnalysisBin) {
      g_object_set (cameraBin, prop, bin, NULL);
      return bin;
    }

    GstElement *analysisBin = gst_element_factory_make ("analysisbin", name);
    if (!analysisBin) {
      qWarning() << "Failed to create analysisbin";
      gst_object_unref(bin);
      return NULL;
    }

    g_object_set (analysisBin, "analysis-filter", bin, NULL);
    g_object_set (analysisBin, "bypass", FALSE, NULL);

    g_object_set (cameraBin, prop, analysisBin, NULL);

    return analysisBin;
  }

#if GST_CHECK_VERSION(1,0,0)
  static gint compare_factory(const GValue *val, const char *factory) {
    GstElement *elem = (GstElement *)g_value_get_object (val);
    gst_object_ref (elem);
#else
  static gint compare_factory(GstElement *elem, const char *factory) {
#endif
    GstElementFactory *f = gst_element_get_factory(elem);
    if (!f) {
      gst_object_unref (elem);
      return -1;
    }

    if (!GST_IS_PLUGIN_FEATURE (f)) {
      gst_object_unref (elem);
      return -1;
    }

    const char *name = gst_plugin_feature_get_name(GST_PLUGIN_FEATURE(f));
    if (!name) {
      gst_object_unref (elem);
      return -1;
    }

    if (strcmp(name, factory) != 0) {
      gst_object_unref (elem);
      return -1;
    }

#if GST_CHECK_VERSION(1,0,0)
    gst_object_unref (elem);
#endif

    return 0;
  }

  GstElement *findByFactory(const char *factory) {
    if (!cameraBin) {
      return NULL;
    }

    GstIterator *iter = gst_bin_iterate_recurse (GST_BIN(cameraBin));

#if GST_CHECK_VERSION(1,0,0)
    GValue val = G_VALUE_INIT;
    GstElement *item = NULL;
    if (gst_iterator_find_custom (iter, (GCompareFunc)compare_factory, &val, (gpointer)factory)) {
      item = (GstElement *)g_value_dup_object (&val);
      g_value_unset (&val);
    }
#else
    GstElement *item = (GstElement *)
      gst_iterator_find_custom (iter, (GCompareFunc)compare_factory, (gpointer)factory);
#endif
    gst_iterator_free (iter);

    return item;
  }

  bool createAndAddViewfinderFilters() {
    QStringList filters = conf->viewfinderFilters();
    if (!filters.isEmpty()) {
      viewfinderFilters = createAndAddFilters("QtCamViewfinderFilters", "viewfinder-filter",
					      filters, conf->viewfinderFiltersUseAnalysisBin());
      return viewfinderFilters != 0;
    }

    return true;
  }

  void createAndAddImageFilters() {
    QStringList filters = conf->imageFilters();
    if (!filters.isEmpty()) {
      createAndAddFilters("QtCamImageFilters", "image-filter",
			  filters, conf->imageFiltersUseAnalysisBin());
    }
  }

  void _d_error(const QString& message, int code, const QString& debug) {
    error = true;

    QMetaObject::invokeMethod(q_ptr, "error", Q_ARG(QString, message),
			      Q_ARG(int, code), Q_ARG(QString, debug));

    if (active) {
      QMetaObject::invokeMethod(active, "canCaptureChanged", Qt::QueuedConnection);
    }
  }

  void _d_stopped() {
    QMetaObject::invokeMethod(q_ptr, "stopped");
  }

  void _d_stopping() {
    if (active) {
      QMetaObject::invokeMethod(active, "canCaptureChanged", Qt::QueuedConnection);
    }

    QMetaObject::invokeMethod(q_ptr, "stopping", Qt::QueuedConnection);
    QMetaObject::invokeMethod(q_ptr, "runningStateChanged", Qt::QueuedConnection,
			      Q_ARG(bool, false));
  }

  void _d_started() {
    if (active) {
      QMetaObject::invokeMethod(active, "canCaptureChanged", Qt::QueuedConnection);
    }

    QMetaObject::invokeMethod(q_ptr, "started", Qt::QueuedConnection);
    QMetaObject::invokeMethod(q_ptr, "runningStateChanged", Qt::QueuedConnection,
			      Q_ARG(bool, true));
  }

  void setAudioCaptureCaps() {
    QString captureCaps = conf->audioCaptureCaps();
    if (!captureCaps.isEmpty()) {
      GstCaps *caps = gst_caps_from_string(captureCaps.toLatin1().data());
      if (caps) {
	g_object_set(cameraBin, "audio-capture-caps", caps, NULL);
	gst_caps_unref(caps);
      }
    }
  }

  bool isReadyForCapture() {
    GstElement *src = videoSource;
    if (wrapperVideoSource) {
      src = wrapperVideoSource;
    }

    if (!src) {
      return false;
    }

    gboolean ready = FALSE;
    g_object_get(src, "ready-for-capture", &ready, NULL);

    return ready == TRUE;
  }

  static void on_ready_for_capture_changed(GObject *obj, GParamSpec *pspec,
					   QtCamDevicePrivate *d)  {
    Q_UNUSED(obj);
    Q_UNUSED(pspec);

    if (!d->active) {
      return;
    }

    QMetaObject::invokeMethod(d->active, "canCaptureChanged", Qt::QueuedConnection);
  }

  static void on_idle_changed(GObject *obj, GParamSpec *pspec, QtCamDevicePrivate *d) {
    Q_UNUSED(obj);
    Q_UNUSED(pspec);

    QMetaObject::invokeMethod(d->q_ptr, "idleStateChanged", Qt::QueuedConnection,
			      Q_ARG(bool, d->q_ptr->isIdle()));
  }

  static void on_sensor_orientation_changed(GObject *obj, GParamSpec *pspec,
					    QtCamDevicePrivate *d) {
    Q_UNUSED(obj);
    Q_UNUSED(pspec);

    QMetaObject::invokeMethod(d->q_ptr, "sensorOrientationAngleChanged", Qt::QueuedConnection);
  }

  QList<QSize> queryResolutions(const char *prop) {
    QList<QSize> res;

    if (!cameraBin) {
#if 0
      qWarning() << "queryResolutions: no camerabin";
#endif
      return res;
    }

    GstCaps *caps = NULL;
    g_object_get(cameraBin, prop, &caps, NULL);

    if (!caps) {
#if 0
      qWarning() << "queryResolutions: no caps";
#endif
      return res;
    }

    for (guint x = 0; x < gst_caps_get_size(caps); x++) {
      const GstStructure *s = gst_caps_get_structure(caps, x);
      const GValue *width = gst_structure_get_value(s, "width");
      const GValue *height = gst_structure_get_value(s, "height");

      if (!width || !height) {
	qWarning() << "queryResolutions: no dimensions";
	continue;
      }

      bool width_is_list = GST_VALUE_HOLDS_LIST(width) ? true : false;
      bool height_is_list = GST_VALUE_HOLDS_LIST(height) ? true : false;

      if (!width_is_list && !height_is_list) {
	res << QSize(g_value_get_int(width), g_value_get_int(height));
      } else if (width_is_list && height_is_list) {
	guint ws = gst_value_list_get_size(width);
	guint hs = gst_value_list_get_size(width);
	for (guint wc = 0; wc < ws; wc++) {
	  int w = g_value_get_int(gst_value_list_get_value(width, wc));
	  for (guint hc = 0; hc < hs; hc++) {
	    int h = g_value_get_int(gst_value_list_get_value(height, hc));
	    res << QSize(w, h);
	  }
	}
      } else if (width_is_list) {
	int h = g_value_get_int(height);
	for (guint i = 0; i < gst_value_list_get_size(width); i++) {
	  int w = g_value_get_int(gst_value_list_get_value(width, i));
	  res << QSize(w, h);
	}
      } else if (height_is_list) {
	int w = g_value_get_int(width);

	for (guint i = 0; i < gst_value_list_get_size(height); i++) {
	  int h = g_value_get_int(gst_value_list_get_value(height, i));
	  res << QSize(w, h);
	}
      }
    }

    gst_caps_unref(caps);

    return res;
  }

  QList<QtCamResolution> generateImageResolutions(const QList<QSize>& vf,
						  const QList<QSize>& image) {

    QList<QtCamResolution> res;
    int fps = conf->resolutionsImageFps();

    foreach (const QSize& i, image) {
      QString aspect = QtCamUtils::aspectRatioForResolution(i);
      QSize viewfinder = QtCamUtils::findMatchingResolution(i, vf);
      if (!viewfinder.isValid()) {
	continue;
      }

      float mp = QtCamUtils::megapixelsForResolution(i);
      QString resolutionId =
	QString("%1x%2-%3").arg(i.width()).arg(i.height()).arg(aspect);

      // For now we use viewfinder as the preview.
      // We also disable night mode and zsl
      QtCamResolution r(resolutionId, aspect, i, viewfinder, viewfinder, fps, -1,
			-1, mp, QString(), QtCamResolution::ModeImage, id);

      res << r;
    }

    if (!res.isEmpty()) {
      QTextStream s(stdout);
      s << "Image resolutions for device " << id.toString() << "\n";
      foreach (const QtCamResolution& r, res) {
	s << r;
      }
    }

    return res;
  }

  QList<QtCamResolution> generateVideoResolutions(const QList<QSize>& vf,
						  const QList<QSize>& video) {
    QList<QtCamResolution> res;
    int fps = conf->resolutionsVideoFps();

    foreach (const QSize& v, video) {
      QString aspect = QtCamUtils::aspectRatioForResolution(v);
      QString commonName = QtCamUtils::commonNameForResolution(v);
      if (commonName.isEmpty()) {
	continue;
      }

      QSize viewfinder = QtCamUtils::findMatchingResolution(v, vf);
      if (!viewfinder.isValid()) {
	continue;
      }

      float mp = QtCamUtils::megapixelsForResolution(v);
      QString resolutionId =
	QString("%1x%2-%3").arg(v.width()).arg(v.height()).arg(aspect);

      // For now we use viewfinder as the preview.
      // We also disable night mode and zsl
      QtCamResolution r(resolutionId, aspect, v, viewfinder, viewfinder, fps, -1,
			-1, mp, commonName, QtCamResolution::ModeVideo, id);

      res << r;
    }

    if (!res.isEmpty()) {
      QTextStream s(stdout);
      s << "Video resolutions for device " << id.toString() << "\n";
      foreach (const QtCamResolution& r, res) {
	s << r;
      }
    }

    return res;
  }

  QString name;
  QVariant id;

  QtCamDevice *q_ptr;

  GstElement *cameraBin;
  GstElement *videoSource;
  GstElement *wrapperVideoSource;
  GstElement *sink;

  QtCamImageMode *image;
  QtCamVideoMode *video;
  QtCamMode *active;

  QtCamViewfinder *viewfinder;
  QtCamConfig *conf;
  QtCamGstMessageListener *listener;
  QtCamViewfinderBufferListener *bufferListener;
  QtCamViewfinderFrameListener *frameListener;
  bool error;
  QtCamNotifications *notifications;
  QtCamPropertySetter *propertySetter;
  GstElement *viewfinderFilters;
  QtCamImageSettings *imageSettings;
  QtCamVideoSettings *videoSettings;
};

#endif /* QT_CAM_DEVICE_P_H */
