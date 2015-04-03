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

#include "qtcamimagemode.h"
#include "qtcammode_p.h"
#include "qtcamdevice_p.h"
#include "qtcamimagesettings.h"
#include "qtcamdevice.h"

class QtCamImageModePrivate : public QtCamModePrivate {
public:
  QtCamImageModePrivate(QtCamDevicePrivate *dev) :
  QtCamModePrivate(dev),
  fastCaptureEnabled(false) {

  }

  ~QtCamImageModePrivate() {
  }

  bool applyFastCapture() {
    GstElement *src = dev->videoSource;
    if (dev->wrapperVideoSource) {
      src = dev->wrapperVideoSource;
    }

    if (!src) {
      qWarning() << "applyFastCapture: no source element";
      return false;
    }

    QString prop = dev->conf->fastCaptureProperty();
    if (prop.isEmpty()) {
      qWarning() << "applyFastCapture: not supported";
      return false;
    }

    if (fastCaptureEnabled) {
      g_object_set(src, prop.toUtf8().constData(), TRUE, NULL);
    } else {
      g_object_set(src, prop.toUtf8().constData(), FALSE, NULL);
    }

    return true;
  }

  QtCamResolution resolution;
  bool fastCaptureEnabled;
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
      setEncodingProfile(profile, QLatin1String("image-profile"));
    }
  }
}

QtCamImageMode::~QtCamImageMode() {
  d = 0;
}

bool QtCamImageMode::canCapture() {
  return QtCamMode::canCapture() && d_ptr->dev->isReadyForCapture();
}

void QtCamImageMode::applySettings() {
  if (!d->resolution.isValid()) {
    d->resolution = settings()->defaultResolution();
  }

  int fps = d->resolution.frameRate();

  if (d_ptr->inNightMode()) {
    int nightFps = d->resolution.nightFrameRate();
    // No warning for night mode because we might have night scene mode enabled
    // even though we use the same frame rate as the other scene modes
    if (nightFps > 0) {
      fps = nightFps;
    }
  } else if (d->fastCaptureEnabled) {
    int zslFps = d->resolution.zslFrameRate();
    if (zslFps > 0) {
      fps = zslFps;
    } else {
      qWarning() << "QtCamImageMode::applySettings: resolution does not support fast capture";
    }
  }

  d_ptr->setCaps("viewfinder-caps", d->resolution.viewfinderResolution(), fps);

  // FIXME:
  // Ideally, we should query the image-capture-supported-caps and get a proper framerate
  // as it seems that subdevsrc2 can only capture 15 FPS for at least the highest resolution
  // we use. For now we will not set any FPS.
  d_ptr->setCaps("image-capture-caps", d->resolution.captureResolution(), -1);

  enablePreview();

  // If we don't reset the caps then: if we switch from video to image then we fail
  // the next time we restart the pipeline.
  d_ptr->resetCaps("video-capture-caps");

  d->applyFastCapture();
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

bool QtCamImageMode::setResolution(const QtCamResolution& resolution) {
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

QtCamImageSettings *QtCamImageMode::settings() const {
  return d_ptr->dev->q_ptr->imageSettings();
}

QtCamResolution QtCamImageMode::currentResolution() {
  return d->resolution;
}

void QtCamImageMode::enablePreview() {
  d_ptr->setPreviewSize(d->resolution.previewResolution());
}

bool QtCamImageMode::enableFastCapture() {
  if (d->fastCaptureEnabled) {
    qWarning() << "fast capture is already enabled";
    return false;
  }

  d->fastCaptureEnabled = true;

  if (d->applyFastCapture()) {
    return true;
  }

  d->fastCaptureEnabled = false;

  return false;
}

void QtCamImageMode::disableFastCapture() {
  if (!d->fastCaptureEnabled) {
    qWarning() << "fast capture is already disabled";
    return;
  }

  d->fastCaptureEnabled = false;

  d->applyFastCapture();
}
