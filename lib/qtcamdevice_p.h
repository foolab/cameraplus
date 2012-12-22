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

#ifndef QT_CAM_DEVICE_P_H
#define QT_CAM_DEVICE_P_H

#include <QDebug>
#include <gst/gst.h>
#include "qtcamconfig.h"
#include "qtcamviewfinder.h"
#include "qtcamdevice.h"
#include "qtcammode.h"
#include "qtcamanalysisbin.h"

class QtCamGStreamerMessageListener;
class QtCamMode;
class QtCamImageMode;
class QtCamVideoMode;
class QtCamPropertySetter;

class QtCamDevicePrivate {
public:
  QtCamDevicePrivate() :
    cameraBin(0),
    videoSource(0),
    wrapperVideoSource(0),
    image(0),
    video(0),
    active(0),
    viewfinder(0),
    conf(0),
    error(false),
    notifications(0) {

  }

  GstElement *createAndAddElement(const QString& elementName, const char *prop, const char *name) {
    GstElement *elem = gst_element_factory_make(elementName.toAscii(), name);
    if (!elem) {
      qWarning() << "Failed to create" << elementName;
      return 0;
    }

    g_object_set(cameraBin, prop, elem, NULL);

    return elem;
  }

  void createAndAddVideoSource() {
    // TODO: rework this function
    GstElement *src, *wrapper;
    QString wrapperSrc = conf->wrapperVideoSource();
    QString prop = conf->wrapperVideoSourceProperty();

    if (!prop.isEmpty() && !wrapperSrc.isEmpty()) {
      wrapper = gst_element_factory_make(wrapperSrc.toAscii(), "QCameraWrapperVideoSrc");
      if (!wrapper) {
	qCritical() << "Failed to create wrapper source" << wrapperSrc;
	return;
      }
    }

    src = gst_element_factory_make(conf->videoSource().toAscii(),
					       "QtCameraVideoSrc");
    if (!src) {
      qCritical() << "Failed to create video source";
      if (wrapper) {
	gst_object_unref(wrapper);
      }
      return;
    }

    if (wrapper) {
      g_object_set(wrapper, prop.toAscii(), src, NULL);
      g_object_set(cameraBin, "camera-source", wrapper, NULL);
    }

    videoSource = src;
    wrapperVideoSource = wrapper;

    if (!id.isValid() || id.isNull()) {
      return;
    }

    if (conf->deviceScannerType() == SCANNER_TYPE_ENUM) {
      int dev = id.toInt();
      g_object_set(src, conf->deviceScannerProperty().toAscii().data(), dev, NULL);
    }
    else {
      QString dev = id.toString();
      g_object_set(src, conf->deviceScannerProperty().toAscii().data(),
		   dev.toAscii().data(), NULL);
    }
  }

  bool setViewfinderSink() {
    GstElement *sink;
    g_object_get(cameraBin, "viewfinder-sink", &sink, NULL);

    if (sink) {
      gst_object_unref(sink);
      return true;
    }

    sink = viewfinder->sinkElement();
    if (!sink) {
      qCritical() << "Failed to create GStreamer sink element";
      return false;
    }

    g_object_set(cameraBin, "viewfinder-sink", sink, NULL);

    return true;
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
      GstCaps *caps = gst_caps_from_string(captureCaps.toAscii().data());
      if (caps) {
	g_object_set(cameraBin, "audio-capture-caps", caps, NULL);
	gst_caps_unref(caps);
      }
    }
  }

  void addViewfinderFilters() {
    addElements("viewfinder-filter", conf->viewfinderFilters());
  }

  bool isWrapperReady() {
    if (!wrapperVideoSource) {
      return false;
    }

    gboolean ready = FALSE;
    g_object_get(wrapperVideoSource, "ready-for-capture", &ready, NULL);

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

  void addElements(const char *prop, const QStringList& elements) {
    QList<GstElement *> list;

    if (elements.isEmpty()) {
      return;
    }

    foreach (const QString& element, elements) {
      GstElement *elem = gst_element_factory_make(element.toUtf8().constData(), NULL);
      if (!elem) {
	qWarning() << "Failed to create element" << element;
      }
      else {
	list << elem;
      }
    }

    if (list.isEmpty()) {
      return;
    }

    GstElement *bin = qt_cam_analysis_bin_create(list, prop);
    if (!bin) {
      qWarning() << "Failed to create bin for" << prop;
      return;
    }

    g_object_set(cameraBin, prop, bin, NULL);
  }

#if 0
  void resetCapabilities() {
    g_object_set(cameraBin, "zoom", 1.0, NULL);
    g_object_set(videoSource, "flash-mode", 0, NULL);

    // TODO: should scene mode be manual or automatic ?
    g_object_set(videoSource, "scene-mode", 0, NULL);
  }
#endif

  QString name;
  QVariant id;

  QtCamDevice *q_ptr;

  GstElement *cameraBin;
  GstElement *videoSource;
  GstElement *wrapperVideoSource;

  QtCamImageMode *image;
  QtCamVideoMode *video;
  QtCamMode *active;

  QtCamViewfinder *viewfinder;
  QtCamConfig *conf;
  QtCamGStreamerMessageListener *listener;
  bool error;
  QtCamNotifications *notifications;
  QtCamPropertySetter *propertySetter;
};

#endif /* QT_CAM_DEVICE_P_H */
