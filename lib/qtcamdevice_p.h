// -*- c++ -*-

#ifndef QT_CAM_DEVICE_P_H
#define QT_CAM_DEVICE_P_H

#include <QDebug>
#include <gst/gst.h>
#include "qtcamconfig.h"
#include "qtcamviewfinder.h"
#include "qtcamdevice.h"
#include "qtcammode.h"

class QtCamGStreamerMessageListener;
class QtCamMode;
class QtCamImageMode;
class QtCamVideoMode;

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
    error(false) {

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
};

#endif /* QT_CAM_DEVICE_P_H */
