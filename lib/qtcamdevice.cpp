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

#include "qtcamdevice.h"
#include "qtcamviewfinder.h"
#include "qtcamconfig.h"
#include "qtcamdevice_p.h"
#include <QDebug>
#include <gst/gst.h>
#include "qtcamgstmessagelistener.h"
#include "qtcammode.h"
#include "qtcamimagemode.h"
#include "qtcamvideomode.h"
#include "qtcamnotifications.h"
#include "qtcampropertysetter.h"
#include "qtcamviewfinderbufferlistener.h"
#include "qtcamviewfinderframelistener.h"
#include "qtcamviewfinderframelistener_p.h"
#include "qtcamconfig_p.h"
#include "qtcamimagesettings.h"
#include "qtcamvideosettings.h"

#define SET_STATE(x)                                                                          \
  {                                                                                           \
    GstStateChangeReturn err = gst_element_set_state(d_ptr->cameraBin, x);                    \
    if (err == GST_STATE_CHANGE_FAILURE) {                                                    \
      qWarning() << "failed to set pipeline state to" << x;                                   \
      return false;                                                                           \
    }                                                                                         \
                                                                                              \
    if (err == GST_STATE_CHANGE_ASYNC) {                                                      \
      /* We need to wait. Harmattan's subdevsrc has a race condition. If we set the scene */  \
      /* mode to night and update the resolutions before the pipeline is running */           \
      /* then it barfs with: streaming task paused, reason not-negotiated (-4) */             \
      GstState state;                                                                         \
      if (gst_element_get_state(d_ptr->cameraBin, &state, NULL, GST_SECOND)                   \
	  != GST_STATE_CHANGE_SUCCESS) {                                                      \
	/* We are seriously screwed up :( */                                                  \
	return false;                                                                         \
      }                                                                                       \
                                                                                              \
      if (state != x) {                                                                       \
	/* Huh ? Is this even possible ?? */                                                  \
	return false;                                                                         \
      }                                                                                       \
    }                                                                                         \
  }

QtCamDevice::QtCamDevice(QtCamConfig *config, const QString& name,
			 const QVariant& id, QObject *parent) :
  QObject(parent),
  d_ptr(new QtCamDevicePrivate) {

  d_ptr->q_ptr = this;
  d_ptr->name = name;
  d_ptr->id = id;
  d_ptr->conf = config;

#if GST_CHECK_VERSION(1,0,0)
  d_ptr->cameraBin = gst_element_factory_make("camerabin", "QtCameraCameraBin");
#else
  d_ptr->cameraBin = gst_element_factory_make("camerabin2", "QtCameraCameraBin");
#endif

  if (!d_ptr->cameraBin) {
    qCritical() << "Failed to create camerabin";
    return;
  }

  d_ptr->propertySetter = new QtCamPropertySetter(d_ptr);

  d_ptr->createAndAddElement(d_ptr->conf->audioSource(), "audio-source", "QtCameraAudioSrc");
  if (!d_ptr->conf->wrapperVideoSource().isEmpty() &&
      !d_ptr->conf->wrapperVideoSourceProperty().isEmpty()) {
    d_ptr->createAndAddVideoSourceAndWrapper();
  }
  else {
    d_ptr->createAndAddVideoSource();
  }

  d_ptr->setDevicePoperty();

  int flags =
    0x00000001 /* no-audio-conversion - Do not use audio conversion elements */
    | 0x00000002 /* no-video-conversion - Do not use video conversion elements */
    | 0x00000004 /* no-viewfinder-conversion - Do not use viewfinder conversion elements */
    | 0x00000008; /* no-image-conversion - Do not use image conversion elements */

  g_object_set(d_ptr->cameraBin, "flags", flags, NULL);

  d_ptr->setAudioCaptureCaps();

  d_ptr->createAndAddImageFilters();

  if (!d_ptr->createAndAddViewfinderFilters()) {
    qWarning() << "Failed to create viewfinder filters";
  }

  d_ptr->bufferListener = new QtCamViewfinderBufferListener(d_ptr, this);
  d_ptr->listener = new QtCamGstMessageListener(gst_element_get_bus(d_ptr->cameraBin),
						d_ptr, this);

  QObject::connect(d_ptr->listener, SIGNAL(error(const QString&, int, const QString&)),
		   d_ptr, SLOT(_d_error(const QString&, int, const QString&)));
  QObject::connect(d_ptr->listener, SIGNAL(started()), d_ptr, SLOT(_d_started()));
  QObject::connect(d_ptr->listener, SIGNAL(stopped()), d_ptr, SLOT(_d_stopped()));
  QObject::connect(d_ptr->listener, SIGNAL(stopping()), d_ptr, SLOT(_d_stopping()));

  g_signal_connect(d_ptr->cameraBin, "notify::idle",
		   G_CALLBACK(QtCamDevicePrivate::on_idle_changed), d_ptr);

  if (d_ptr->wrapperVideoSource) {
    g_signal_connect(d_ptr->wrapperVideoSource, "notify::ready-for-capture",
		     G_CALLBACK(QtCamDevicePrivate::on_ready_for_capture_changed), d_ptr);
  }
  else {
    g_signal_connect(d_ptr->videoSource, "notify::ready-for-capture",
		     G_CALLBACK(QtCamDevicePrivate::on_ready_for_capture_changed), d_ptr);
  }

  if (d_ptr->videoSource) {
    g_signal_connect(d_ptr->videoSource, "notify::sensor-orientation",
		     G_CALLBACK(QtCamDevicePrivate::on_sensor_orientation_changed), d_ptr);
  }

  d_ptr->image = new QtCamImageMode(d_ptr, this);
  d_ptr->video = new QtCamVideoMode(d_ptr, this);

  d_ptr->notifications = new QtCamNotifications(this, this);
  d_ptr->frameListener = new QtCamViewfinderFrameListener(this);
}

QtCamDevice::~QtCamDevice() {
  stop(true);

  d_ptr->bufferListener->d_ptr->setSink(0);

  d_ptr->image->deactivate();
  d_ptr->video->deactivate();

  delete d_ptr->image; d_ptr->image = 0;
  delete d_ptr->video; d_ptr->video = 0;

  delete d_ptr->propertySetter;

  if (d_ptr->cameraBin) {
    gst_object_unref(d_ptr->cameraBin);
  }

  delete d_ptr->imageSettings; d_ptr->imageSettings = 0;
  delete d_ptr->videoSettings; d_ptr->videoSettings = 0;

  delete d_ptr; d_ptr = 0;
}

bool QtCamDevice::setViewfinder(QtCamViewfinder *viewfinder) {
  if (isRunning()) {
    qWarning() << "QtCamDevice: pipeline must be stopped before setting a viewfinder";
    return false;
  }

  if (d_ptr->viewfinder == viewfinder) {
    return true;
  }

  if (!viewfinder) {
    if (d_ptr->cameraBin) {
      g_object_set(d_ptr->cameraBin, "viewfinder-sink", NULL, NULL);
    }

    d_ptr->sink = 0;
    d_ptr->bufferListener->d_ptr->setSink(0);
    d_ptr->viewfinder = 0;
    return true;
  }

  d_ptr->viewfinder = viewfinder;
  d_ptr->sink = 0;
  d_ptr->bufferListener->d_ptr->setSink(0);

  return true;
}

QtCamViewfinder *QtCamDevice::viewfinder() const {
  return d_ptr->viewfinder;
}

bool QtCamDevice::start() {
  if (d_ptr->error) {
    qWarning() << "Pipeline must be stopped first because of an error.";
    return false;
  }

  if (!d_ptr->cameraBin) {
    qWarning() << "Missing camerabin";
    return false;
  }

  if (!d_ptr->viewfinder) {
    qWarning() << "Viewfinder not set";
    return false;
  }

  if (isRunning()) {
    return true;
  }

  // start viewfinder.
  d_ptr->viewfinder->start();

  // Set sink.
  if (!d_ptr->setViewfinderSink()) {
    return false;
  }

  // Go to paused state:
  SET_STATE(GST_STATE_PAUSED);

  // Now query the resolutions if needed
  if (d_ptr->conf->resolutionsProvider() != RESOLUTIONS_PROVIDER_INI) {
    if (!d_ptr->imageSettings->hasResolutions()) {
      QList<QtCamResolution> resolutions =
	d_ptr->generateImageResolutions(queryViewfinderResolutions(), queryImageResolutions());
      d_ptr->imageSettings->updateResolutions(resolutions);
    }

    if (!d_ptr->videoSettings->hasResolutions()) {
      QList<QtCamResolution> resolutions =
	d_ptr->generateVideoResolutions(queryViewfinderResolutions(), queryVideoResolutions());
      d_ptr->videoSettings->updateResolutions(resolutions);
    }
  }

  if (!d_ptr->active) {
    d_ptr->image->activate();
  }
  else {
    d_ptr->active->applySettings();
  }

  SET_STATE(GST_STATE_PLAYING);

  d_ptr->frameListener->d_ptr->setRenderer(d_ptr->viewfinder->renderer());

  return true;
}

bool QtCamDevice::stop(bool force) {
  if (!d_ptr->cameraBin) {
    return true;
  }

  d_ptr->frameListener->d_ptr->setRenderer(0);

  if (d_ptr->error) {
    gst_element_set_state(d_ptr->cameraBin, GST_STATE_NULL);
    d_ptr->error = false;

    d_ptr->viewfinder->stop();

    return true;
  }

  GstState state;
  gst_element_get_state(d_ptr->cameraBin, &state, 0, GST_CLOCK_TIME_NONE);

  if (state == GST_STATE_NULL) {
    // Nothing to do.
    return true;
  }

  if (!isIdle()) {
    if (!force) {
      return false;
    }
  }

  // First we go to ready:
  GstStateChangeReturn st = gst_element_set_state(d_ptr->cameraBin, GST_STATE_READY);
  if (st != GST_STATE_CHANGE_FAILURE) {
    // Flush the bus:
    d_ptr->listener->flushMessages();
  }

  // Now to NULL
  gst_element_set_state(d_ptr->cameraBin, GST_STATE_NULL);

  d_ptr->viewfinder->stop();

  return true;
}

bool QtCamDevice::isRunning() {
  if (!d_ptr->cameraBin) {
    return false;
  }

  GstState state;
  GstStateChangeReturn err = gst_element_get_state(d_ptr->cameraBin,
						   &state, 0, GST_CLOCK_TIME_NONE);

  if (err == GST_STATE_CHANGE_FAILURE || state != GST_STATE_PLAYING) {
    return false;
  }

  return true;
}

bool QtCamDevice::isIdle() {
  if (!d_ptr->cameraBin) {
    return true;
  }

  gboolean idle = FALSE;
  g_object_get(d_ptr->cameraBin, "idle", &idle, NULL);

  return idle == TRUE;
}

QtCamImageMode *QtCamDevice::imageMode() const {
  return d_ptr->image;
}

QtCamVideoMode *QtCamDevice::videoMode() const {
  return d_ptr->video;
}

QtCamMode *QtCamDevice::activeMode() const {
  return d_ptr->active;
}

QString QtCamDevice::name() const {
  return d_ptr->name;
}

QVariant QtCamDevice::id() const {
  return d_ptr->id;
}

QtCamConfig *QtCamDevice::config() const {
  return d_ptr->conf;
}

QtCamGstMessageListener *QtCamDevice::listener() const {
  return d_ptr->listener;
}

QtCamViewfinderBufferListener *QtCamDevice::bufferListener() const {
  return d_ptr->bufferListener;
}

QtCamViewfinderFrameListener *QtCamDevice::frameListener() const {
  return d_ptr->frameListener;
}

QtCamNotifications *QtCamDevice::notifications() const {
  return d_ptr->notifications;
}

int QtCamDevice::sensorOrientationAngle() {
  // TODO: this is Sailfish OS specific.
  if (d_ptr->videoSource) {
    GObjectClass *klass = G_OBJECT_GET_CLASS(d_ptr->videoSource);
    if (g_object_class_find_property (klass, "sensor-orientation")) {
      int angle = -1;
      g_object_get(d_ptr->videoSource, "sensor-orientation", &angle, NULL);
      return angle;
    }
  }

  return -1;
}

QList<QSize> QtCamDevice::queryImageResolutions() {
  return d_ptr->queryResolutions("image-capture-supported-caps");
}

QList<QSize> QtCamDevice::queryVideoResolutions() {
  return d_ptr->queryResolutions("video-capture-supported-caps");
}

QList<QSize> QtCamDevice::queryViewfinderResolutions() {
  return d_ptr->queryResolutions("viewfinder-supported-caps");
}

QtCamImageSettings *QtCamDevice::imageSettings() {
  if (!d_ptr->imageSettings) {
    QtCamConfigPrivate *conf = d_ptr->conf->d_ptr;

    QString generic = "image";
    QString specific = QString("%1-%2").arg(generic).arg(id().toString());
    QString profileName = conf->readWithFallback(generic, specific, "profile-name").toString();
    QString profilePath = conf->readWithFallback(generic, specific, "profile-path").toString();
    QString suffix = conf->readWithFallback(generic, specific, "extension").toString();

    QList<QtCamResolution> resolutions;

    if (d_ptr->conf->resolutionsProvider() == RESOLUTIONS_PROVIDER_INI) {
      resolutions = conf->readResolutions(QtCamResolution::ModeImage, id());
    } else {
      resolutions =
	d_ptr->generateImageResolutions(queryViewfinderResolutions(), queryImageResolutions());
    }

    d_ptr->imageSettings =
      new QtCamImageSettings(id().toString(), suffix, profileName, profilePath, resolutions, this);
  }

  return d_ptr->imageSettings;
}

QtCamVideoSettings *QtCamDevice::videoSettings() {
  if (!d_ptr->videoSettings) {
    QtCamConfigPrivate *conf = d_ptr->conf->d_ptr;

    QString generic = "video";
    QString specific = QString("%1-%2").arg(generic).arg(id().toString());

    QString profileName = conf->readWithFallback(generic, specific, "profile-name").toString();
    QString profilePath = conf->readWithFallback(generic, specific, "profile-path").toString();
    QString suffix = conf->readWithFallback(generic, specific, "extension").toString();

    QList<QtCamResolution> resolutions;

    if (d_ptr->conf->resolutionsProvider() == RESOLUTIONS_PROVIDER_INI) {
      resolutions = conf->readResolutions(QtCamResolution::ModeVideo, id());
    } else {
      resolutions =
	d_ptr->generateVideoResolutions(queryViewfinderResolutions(), queryVideoResolutions());
    }

    d_ptr->videoSettings =
      new QtCamVideoSettings(id().toString(), suffix, profileName, profilePath, resolutions, this);
  }

  return d_ptr->videoSettings;
}
