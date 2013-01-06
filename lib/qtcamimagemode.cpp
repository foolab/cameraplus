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

#include "qtcamimagemode.h"
#include "qtcammode_p.h"
#include "qtcamdevice_p.h"
#include "qtcamimagesettings.h"
#include "qtcamdevice.h"

class QtCamImageModePrivate : public QtCamModePrivate {
public:
  QtCamImageModePrivate(QtCamDevicePrivate *dev) :
  QtCamModePrivate(dev),
  settings(dev->conf->imageSettings(dev->id)),
  resolution(settings->defaultResolution()) {

  }

  ~QtCamImageModePrivate() {
    delete settings;
  }

  QtCamImageSettings *settings;
  QtCamImageResolution resolution;
};

QtCamImageMode::QtCamImageMode(QtCamDevicePrivate *dev, QObject *parent) :
  QtCamMode(new QtCamImageModePrivate(dev), "mode-image", parent) {

  d_ptr->init(new DoneHandler(d_ptr, "image-done", this));

  d = (QtCamImageModePrivate *)d_ptr;

  QString name = d_ptr->dev->conf->imageEncodingProfileName();
  QString path = d_ptr->dev->conf->imageEncodingProfilePath();

  if (!name.isEmpty() && !path.isEmpty()) {
    GstEncodingProfile *profile = d_ptr->loadProfile(path, name);
    if (profile) {
      setProfile(profile);
    }
  }
}

QtCamImageMode::~QtCamImageMode() {
  d = 0;
}

bool QtCamImageMode::canCapture() {
  return QtCamMode::canCapture() && d_ptr->dev->isWrapperReady();
}

void QtCamImageMode::applySettings() {
  bool night = d_ptr->inNightMode();

  int fps = night ? d->resolution.nightFrameRate() : d->resolution.frameRate();

  d_ptr->setCaps("viewfinder-caps", d->resolution.viewfinderResolution(), fps);

  // FIXME:
  // Ideally, we should query the image-capture-supported-caps and get a proper framerate
  // as it seems that subdevsrc2 can only capture 15 FPS for at least the highest resolution
  // we use. For now we will not set any FPS.
  d_ptr->setCaps("image-capture-caps", d->resolution.captureResolution(), -1);

  d_ptr->setPreviewSize(d->resolution.previewResolution());

  // If we don't reset the caps then: if we switch from video to image then we fail
  // the next time we restart the pipeline.
  d_ptr->resetCaps("video-capture-caps");
}

void QtCamImageMode::start() {
  d_ptr->enableViewfinderFilters();
}

void QtCamImageMode::stop() {
  // Nothing
}

bool QtCamImageMode::capture(const QString& fileName) {
  if (!canCapture()) {
    return false;
  }

  if (fileName.isEmpty()) {
    return false;
  }

  d_ptr->setFileName(fileName);

  g_object_set(d_ptr->dev->cameraBin, "location", fileName.toUtf8().data(), NULL);
  g_signal_emit_by_name(d_ptr->dev->cameraBin, "start-capture", NULL);

  return true;
}

bool QtCamImageMode::setResolution(const QtCamImageResolution& resolution) {
  d->resolution = resolution;

  if (!d_ptr->dev->q_ptr->isRunning()) {
    // We will return true here because setting the resolution on a non-running pipeline
    // doesn't make much sense (Probably the only use case is as a kind of optimization only).
    // We will set it anyway when the pipeline gets started.
    return true;
  }

  if (!d_ptr->dev->q_ptr->isIdle()) {
    return false;
  }

  applySettings();

  return true;
}

void QtCamImageMode::setProfile(GstEncodingProfile *profile) {
  if (!d_ptr->dev->cameraBin) {
    gst_encoding_profile_unref(profile);
    return;
  }

  g_object_set(d_ptr->dev->cameraBin, "image-profile", profile, NULL);
}

QtCamImageSettings *QtCamImageMode::settings() const {
  return d->settings;
}
