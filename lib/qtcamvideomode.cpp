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

class StreamRewriter {
public:
  StreamRewriter(GstPad *srcPad, bool copy) {
    ts_a = ts_b = ts_delta = GST_CLOCK_TIME_NONE;

    updateTs = false;
    blocked = false;
    updateDelta = false;
    copyBuffers = copy;
#if GST_CHECK_VERSION(1,0,0)
    probe = gst_pad_add_probe(srcPad, GST_PAD_PROBE_TYPE_BUFFER, gst1_buffer_probe, this, NULL);
#else
    probe = gst_pad_add_buffer_probe(srcPad, G_CALLBACK(buffer_probe), this);
#endif
    pad = srcPad;
  }

  ~StreamRewriter() {
#if GST_CHECK_VERSION(1,0,0)
    gst_pad_remove_probe(pad, probe);
#else
    gst_pad_remove_buffer_probe(pad, probe);
#endif
    gst_object_unref(pad);
  }

  void block() {
    QMutexLocker locker(&mutex);
    updateTs = true;
    blocked = true;
  }

  void unblock() {
    QMutexLocker locker(&mutex);
    updateDelta = true;
    blocked = false;
  }

  bool isBlocked() {
    return blocked;
  }

private:
#if GST_CHECK_VERSION(1,0,0)
  static GstPadProbeReturn gst1_buffer_probe(GstPad *pad, GstPadProbeInfo *info,
					     gpointer user_data) {
    if (_buffer_probe(pad, (GstBuffer *)info->data, user_data)) {
      return GST_PAD_PROBE_PASS;
    }

    return GST_PAD_PROBE_DROP;
  }
#endif

  static gboolean buffer_probe(GstPad *pad, GstMiniObject *mini_obj, gpointer user_data) {
    GstBuffer *buffer = GST_BUFFER(mini_obj);
    return _buffer_probe(pad, buffer, user_data);
  }

  static gboolean _buffer_probe(GstPad *pad, GstBuffer *buffer, gpointer user_data) {
#if GST_CHECK_VERSION(1,0,0)
    Q_UNUSED(pad);
#endif

    StreamRewriter *rewriter = (StreamRewriter *) user_data;

    QMutexLocker locker(&rewriter->mutex);

    // The whole logic is from here:
    // http://stackoverflow.com/questions/7524658/how-to-pause-video-recording-in-gstreamer
    if (rewriter->blocked) {
      if (rewriter->updateTs) {
	rewriter->ts_a = GST_BUFFER_TIMESTAMP(buffer);
	rewriter->updateTs = false;
      }

      rewriter->ts_b = GST_BUFFER_TIMESTAMP(buffer);

      return FALSE;
    }
    if (rewriter->updateDelta) {
      GstClockTime delta = rewriter->ts_b - rewriter->ts_a;
      rewriter->ts_delta += delta;
      rewriter->updateDelta = false;
    }

    if (rewriter->copyBuffers) {
#if GST_CHECK_VERSION(1,0,0)
      // The only GStreamer 1.x implementation we have currently is SailfishOS droidcamsrc
      // which pushes different buffers so no need to copy. It's bad that we manipulate
      // the buffer in place like that though :/

      if (GST_CLOCK_TIME_IS_VALID(GST_BUFFER_DTS(buffer))) {
	GST_BUFFER_DTS(buffer) -= rewriter->ts_delta;
      }

      if (GST_CLOCK_TIME_IS_VALID(GST_BUFFER_PTS(buffer))) {
	GST_BUFFER_PTS(buffer) -= rewriter->ts_delta;
      }

      return TRUE;
#else
      // The only implementation we have for GStreamer 0.10 is Harmattan subdevsrc
      // which pushes the same frame through vidsrc and vfsrc so we must copy the buffer
      GstBuffer *new_buffer = gst_buffer_new();
      GST_BUFFER_DATA(new_buffer) = GST_BUFFER_DATA(buffer);
      GST_BUFFER_SIZE(new_buffer) = GST_BUFFER_SIZE(buffer);
      GST_BUFFER_TIMESTAMP(new_buffer) = GST_BUFFER_TIMESTAMP(buffer) - rewriter->ts_delta;
      GST_BUFFER_DURATION(new_buffer) = GST_BUFFER_DURATION(buffer);
      GST_BUFFER_OFFSET(new_buffer) = GST_BUFFER_OFFSET(buffer);
      GST_BUFFER_OFFSET_END(new_buffer) = GST_BUFFER_OFFSET_END(buffer);
      GST_BUFFER_MALLOCDATA(new_buffer) = (guint8 *)gst_buffer_ref(buffer);
      GST_BUFFER_FREE_FUNC(new_buffer) = (void (*)(void*))gst_buffer_unref;

      // TODO: error checking
      gst_pad_chain(pad->peer, new_buffer);

      return FALSE; // drop
#endif
    } else {
#if GST_CHECK_VERSION(1,0,0)
      if (GST_CLOCK_TIME_IS_VALID(GST_BUFFER_DTS(buffer))) {
	GST_BUFFER_DTS(buffer) -= rewriter->ts_delta;
      }

      if (GST_CLOCK_TIME_IS_VALID(GST_BUFFER_PTS(buffer))) {
	GST_BUFFER_PTS(buffer) -= rewriter->ts_delta;
      }
#else
      GST_BUFFER_TIMESTAMP(buffer) -= rewriter->ts_delta;
#endif

      return TRUE; // pass
    }
  }

  GstPad *pad;
  gulong probe;
  GstClockTime ts_a, ts_b, ts_delta;
  bool blocked;
  bool updateTs;
  bool updateDelta;
  bool copyBuffers;
  QMutex mutex;
};

class QtCamVideoModePrivate : public QtCamModePrivate {
public:
  QtCamVideoModePrivate(QtCamDevicePrivate *dev) :
  QtCamModePrivate(dev),
  resolution(QtCamResolution(QtCamResolution::ModeVideo)),
  audio(0),
  video(0) {

  }

  ~QtCamVideoModePrivate() {
  }

  void _d_idleStateChanged(bool isIdle) {
    if (isIdle && dev->active == dev->video) {
      QMetaObject::invokeMethod(dev->video, "recordingStateChanged");
      QMetaObject::invokeMethod(dev->video, "canCaptureChanged");
    }
  }

  StreamRewriter *createRewriter(const char *prop, const char *name, bool copy) {
    GstPad *pad = NULL;
    GstElement *elem = NULL;

    g_object_get(dev->cameraBin, prop, &elem, NULL);

    if (!elem) {
      qWarning() << "Failed to get element" << prop;
      return NULL;
    }

    pad = gst_element_get_static_pad(elem, name);
    if (!pad) {
      qWarning() << "Failed to get pad" << name << "from element" << prop;
    }

    gst_object_unref(elem);

    if (pad) {
      return new StreamRewriter(pad, copy);
    }

    return NULL;
  }

  void createRewriters() {
    if (!audio) {
      audio = createRewriter("audio-source", "src", false);
    }

    if (!video) {
      video = createRewriter("camera-source", "vidsrc", true);
    }
  }

  void clearRewriters() {
    if (audio) {
      delete audio;
      audio = 0;
    }

    if (video) {
      delete video;
      video = 0;
    }
  }

  QtCamResolution resolution;

  StreamRewriter *audio;
  StreamRewriter *video;
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
    emit resolutionChanged();
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

bool QtCamVideoMode::isPaused() {
  if (!isRecording()) {
    return false;
  }

  if ((d->audio && d->audio->isBlocked()) || (d->video && d->video->isBlocked())) {
    return true;
  }

  return false;
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
  pauseRecording(false);

  if (isRecording()) {
    VideoDoneHandler *handler = dynamic_cast<VideoDoneHandler *>(d_ptr->doneHandler);
    if (sync) {
      handler->lock();

      if (handler->isDone()) {
	handler->unlock();
	d->clearRewriters();
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

  d->clearRewriters();
}

void QtCamVideoMode::pauseRecording(bool pause) {
  if (!isRecording()) {
    return;
  }

  if (isPaused() == pause) {
    return;
  }

  if (pause) {
    d->createRewriters();
    d->audio->block();
    d->video->block();
  } else {
    d->audio->unblock();
    d->video->unblock();
  }

  emit pauseStateChanged();
}

bool QtCamVideoMode::setResolution(const QtCamResolution& resolution) {
  d->resolution = resolution;
  emit resolutionChanged();

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
