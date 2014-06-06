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
#include "qtcamgstreamermessagelistener.h"
#include "qtcammode.h"
#include "qtcamimagemode.h"
#include "qtcamvideomode.h"
#include "qtcamnotifications.h"
#include "qtcampropertysetter.h"

QtCamDevice::QtCamDevice(QtCamConfig *config, const QString& name,
			 const QVariant& id, QObject *parent) :
  QObject(parent), d_ptr(new QtCamDevicePrivate) {

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

  d_ptr->listener = new QtCamGStreamerMessageListener(gst_element_get_bus(d_ptr->cameraBin),
						      d_ptr, this);

  QObject::connect(d_ptr->listener, SIGNAL(error(const QString&, int, const QString&)),
		   this, SLOT(_d_error(const QString&, int, const QString&)));
  QObject::connect(d_ptr->listener, SIGNAL(started()), this, SLOT(_d_started()));
  QObject::connect(d_ptr->listener, SIGNAL(stopped()), this, SLOT(_d_stopped()));
  QObject::connect(d_ptr->listener, SIGNAL(stopping()), this, SLOT(_d_stopping()));

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
}

QtCamDevice::~QtCamDevice() {
  stop(true);

  d_ptr->image->deactivate();
  d_ptr->video->deactivate();

  delete d_ptr->image; d_ptr->image = 0;
  delete d_ptr->video; d_ptr->video = 0;

  delete d_ptr->propertySetter;

  if (d_ptr->cameraBin) {
    gst_object_unref(d_ptr->cameraBin);
  }

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

    d_ptr->viewfinder = 0;
    return true;
  }

  d_ptr->viewfinder = viewfinder;
  d_ptr->sink = 0;

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

  if (!d_ptr->active) {
    d_ptr->image->activate();
  }
  else {
    d_ptr->active->applySettings();
  }

  // start viewfinder.
  d_ptr->viewfinder->start();

  // Set sink.
  if (!d_ptr->setViewfinderSink()) {
    return false;
  }

  GstStateChangeReturn err = gst_element_set_state(d_ptr->cameraBin, GST_STATE_PLAYING);
  if (err == GST_STATE_CHANGE_FAILURE) {
    qWarning() << "Failed to start camera pipeline";
    return false;
  }

  // We need to wait for startup to complet. There's a race condition somewhere in the pipeline.
  // If we set the scene mode to night and update the resolution while starting up
  // then subdevsrc2 barfs:
  // streaming task paused, reason not-negotiated (-4)
  GstState state;
  if (err != GST_STATE_CHANGE_ASYNC) {
    return true;
  }

  if (gst_element_get_state(d_ptr->cameraBin, &state, 0, GST_CLOCK_TIME_NONE)
      != GST_STATE_CHANGE_SUCCESS) {
    // We are seriously screwed up :(
    return false;
  }

  if (state != GST_STATE_PLAYING) {
    // Huh ? Is this even possible ??
    return false;
  }

  return true;
}

bool QtCamDevice::stop(bool force) {
  if (!d_ptr->cameraBin) {
    return true;
  }

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

QtCamGStreamerMessageListener *QtCamDevice::listener() const {
  return d_ptr->listener;
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

#include "moc_qtcamdevice.cpp"
