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

#include "qtcamvideomode.h"
#include "qtcammode_p.h"
#include <QDebug>
#include "qtcamdevice_p.h"
#include "qtcamdevice.h"
#include "qtcamvideosettings.h"
#include "qtcamnotifications.h"
#include <QMutex>
#include <QWaitCondition>

class QtCamVideoModePrivate : public QtCamModePrivate {
public:
  QtCamVideoModePrivate(QtCamDevicePrivate *dev) :
  QtCamModePrivate(dev),
  resolution(QtCamResolution(QtCamResolution::ModeVideo)) {

  }

  ~QtCamVideoModePrivate() {
  }

  void _d_idleStateChanged(bool isIdle) {
    if (isIdle && dev->active == dev->video) {
      QMetaObject::invokeMethod(dev->video, "recordingStateChanged");
      QMetaObject::invokeMethod(dev->video, "canCaptureChanged");
    }
  }

  QtCamResolution resolution;
};

class VideoDoneHandler : public DoneHandler {
public:
  VideoDoneHandler(QtCamModePrivate *d, QObject *parent = 0) :
    DoneHandler(d, "video-done", parent), m_done(false) {}

  virtual void handleMessage(GstMessage *message) {
    DoneHandler::handleMessage(message);
    wake();
  }

  void lock() {
    m_mutex.lock();
  }

  void unlock() {
    m_mutex.unlock();
  }

  void wait() {
    m_cond.wait(&m_mutex);
  }

  void reset() {
    m_done = false;
  }

  bool isDone() {
    return m_done;
  }

private:
  void wake() {
    lock();
    m_done = true;
    m_cond.wakeOne();
    unlock();
  }

  bool m_done;
  QMutex m_mutex;
  QWaitCondition m_cond;
};

QtCamVideoMode::QtCamVideoMode(QtCamDevicePrivate *dev, QObject *parent) :
  QtCamMode(new QtCamVideoModePrivate(dev), "mode-video", parent) {

  d_ptr->init(new VideoDoneHandler(d_ptr, this));

  d = (QtCamVideoModePrivate *)QtCamMode::d_ptr;

  QString name = d_ptr->dev->conf->videoEncodingProfileName();
  QString path = d_ptr->dev->conf->videoEncodingProfilePath();

  if (!name.isEmpty() && !path.isEmpty()) {
    GstEncodingProfile *profile = d_ptr->loadProfile(path, name);
    if (profile) {
      setEncodingProfile(profile, QLatin1String("video-profile"));
    }
  }

  QObject::connect(d_ptr->dev->q_ptr, SIGNAL(idleStateChanged(bool)),
		   this, SLOT(_d_idleStateChanged(bool)));
}

QtCamVideoMode::~QtCamVideoMode() {
  d = 0;
}

bool QtCamVideoMode::canCapture() {
  return QtCamMode::canCapture() && d_ptr->dev->q_ptr->isIdle();
}

void QtCamVideoMode::applySettings() {
  if (!d->resolution.isValid()) {
    d->resolution = settings()->defaultResolution();
  }

  bool night = d_ptr->inNightMode();

  int fps = night ? d->resolution.nightFrameRate() : d->resolution.frameRate();

  d_ptr->setCaps("viewfinder-caps", d->resolution.viewfinderResolution(), fps);

  d_ptr->setCaps("video-capture-caps", d->resolution.captureResolution(), fps);

  enablePreview();

  // Not sure this is needed but just in case.
  d_ptr->resetCaps("image-capture-caps");
}

void QtCamVideoMode::start() {
  d_ptr->disableViewfinderFilters();
}

void QtCamVideoMode::stop() {
  if (isRecording()) {
    stopRecording(true);
  }
}

bool QtCamVideoMode::isRecording() {
  return !d_ptr->dev->q_ptr->isIdle();
}

bool QtCamVideoMode::startRecording(const QString& fileName, const QString& tmpFileName) {
  if (!canCapture() || isRecording()) {
    return false;
  }

  if (fileName.isEmpty()) {
    return false;
  }

  d_ptr->setFileName(fileName);
  d_ptr->setTempFileName(tmpFileName);

  QString file = tmpFileName.isEmpty() ? fileName : tmpFileName;

  QMetaObject::invokeMethod(d_ptr->dev->notifications, "videoRecordingStarted");

  g_object_set(d_ptr->dev->cameraBin, "location", file.toUtf8().data(), NULL);
  g_signal_emit_by_name(d_ptr->dev->cameraBin, "start-capture", NULL);

  VideoDoneHandler *handler = dynamic_cast<VideoDoneHandler *>(d_ptr->doneHandler);
  handler->reset();

  emit recordingStateChanged();

  emit canCaptureChanged();

  return true;
}

void QtCamVideoMode::stopRecording(bool sync) {
  if (isRecording()) {
    VideoDoneHandler *handler = dynamic_cast<VideoDoneHandler *>(d_ptr->doneHandler);
    if (sync) {
      handler->lock();

      if (handler->isDone()) {
	handler->unlock();
	return;
      }
    }

    g_signal_emit_by_name(d_ptr->dev->cameraBin, "stop-capture", NULL);

    if (sync) {
      // TODO: this can block forever
      handler->wait();
      handler->unlock();
    }
  }
}

bool QtCamVideoMode::setResolution(const QtCamResolution& resolution) {
  d->resolution = resolution;

  if (!d_ptr->dev->q_ptr->isRunning()) {
    // We will return true here because setting the resolution on a non-running pipeline
    // doesn't make much sense (Probably the only use case is as a kind of optimization only).
    // We will set it anyway when the pipeline gets started.
    return true;
  }

  if (isRecording()) {
    return false;
  }

  applySettings();

  return true;
}

QtCamVideoSettings *QtCamVideoMode::settings() {
  return d_ptr->dev->q_ptr->videoSettings();
}

QtCamResolution QtCamVideoMode::currentResolution() {
  return d->resolution;
}

void QtCamVideoMode::enablePreview() {
  d_ptr->setPreviewSize(d->resolution.previewResolution());
}

#include "moc_qtcamvideomode.cpp"
