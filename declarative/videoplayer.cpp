/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
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

#include "videoplayer.h"
#include <QDeclarativeInfo>
#include "qtcamgraphicsviewfinder.h"
#include "cameraconfig.h"
#include <QTimer>

VideoPlayer::VideoPlayer(QDeclarativeItem *parent) :
  QDeclarativeItem(parent),
  m_config(0),
  m_vf(0),
  m_bin(0),
  m_state(VideoPlayer::StateStopped),
  m_timer(new QTimer(this)),
  m_pos(0) {

  m_timer->setSingleShot(false);
  m_timer->setInterval(500);
  QObject::connect(m_timer, SIGNAL(timeout()), this, SIGNAL(positionChanged()));
}

VideoPlayer::~VideoPlayer() {
  stop();

  if (m_bin) {
    gst_object_unref(m_bin);
    m_bin = 0;
  }
}

void VideoPlayer::componentComplete() {

}

void VideoPlayer::classBegin() {
  m_bin = gst_element_factory_make ("playbin2", "VideoPlayerBin");
  if (!m_bin) {
    qmlInfo(this) << "Failed to create playbin2";
    return;
  }

  g_object_set (m_bin, "flags", 99, NULL);

  GstElement *elem = gst_element_factory_make("pulsesink", "VideoPlayerPulseSink");
  if (!elem) {
    qmlInfo(this) << "Failed to create pulsesink";
  }
  else {
    // TODO: properties on sink
    g_object_set (m_bin, "audio-sink", elem, NULL);
  }

  GstBus *bus = gst_element_get_bus(m_bin);
  gst_bus_add_watch(bus, bus_call, this);
  gst_object_unref(bus);
}

QUrl VideoPlayer::source() const {
  return m_url;
}

void VideoPlayer::setSource(const QUrl& source) {
  if (m_url != source) {
    m_url = source;
    emit sourceChanged();
  }
}

CameraConfig *VideoPlayer::cameraConfig() const {
  return m_config;  
}

void VideoPlayer::setCameraConfig(CameraConfig *config) {
  if (m_config && m_config != config) {
    qmlInfo(this) << "Cannot reset CameraConfig";
    return;
  }

  if (!config) {
    qmlInfo(this) << "CameraConfig cannot be empty";
    return;
  }

  if (m_config != config) {
    m_config = config;
    // TODO: We need fence sync here.
    m_vf = new QtCamGraphicsViewfinder(m_config->config(), this);
    m_vf->resize(QSizeF(width(), height()));
    emit cameraConfigChanged();
  }

  if (m_bin) {
    g_object_set(m_bin, "video-sink", m_vf->sinkElement(), NULL);
  }
}

qint64 VideoPlayer::duration() const {
  if (!m_bin) {
    return 0;
  }

  GstFormat format = GST_FORMAT_TIME;
  qint64 dur = 0;
  if (!gst_element_query_duration(m_bin, &format, &dur)) {
    qmlInfo(this) << "Failed to query pipeline duration";
    return 0;
  }

  if (format != GST_FORMAT_TIME) {
    qmlInfo(this) << "Pipeline format is not time";
    return 0;
  }

  dur /= 1000000;

  return dur;
}

qint64 VideoPlayer::position() {
  if (!m_bin) {
    return 0;
  }

  GstFormat format = GST_FORMAT_TIME;
  qint64 pos = 0;
  if (!gst_element_query_position(m_bin, &format, &pos)) {
    qmlInfo(this) << "Failed to query pipeline position";
    return m_pos;
  }

  if (format != GST_FORMAT_TIME) {
    qmlInfo(this) << "Pipeline format is not time";
    return m_pos;
  }

  pos /= 1000000;

  m_pos = pos;

  return pos;
}

void VideoPlayer::setPosition(qint64 position) {
  seek(position);
}

bool VideoPlayer::pause() {
  return setState(VideoPlayer::StatePaused);
}

bool VideoPlayer::play() {
  return setState(VideoPlayer::StatePlaying);
}

bool VideoPlayer::seek(qint64 offset) {
  if (!m_bin) {
    qmlInfo(this) << "no playbin2";
    return false;
  }

  qint64 pos = offset;

  offset *= 1000000;

  GstSeekFlags flags = (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE);
  gboolean ret = gst_element_seek_simple (m_bin, GST_FORMAT_TIME,
					  flags, offset);

  if (ret) {
    m_pos = pos;

    return TRUE;
  }

  return FALSE;
}

bool VideoPlayer::stop() {
  return setState(VideoPlayer::StateStopped);
}

void VideoPlayer::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);

  if (m_vf) {
    m_vf->resize(newGeometry.size());
  }
}

VideoPlayer::State VideoPlayer::state() const {
  return m_state;
}

bool VideoPlayer::setState(const VideoPlayer::State& state) {
  if (state == m_state) {
    return true;
  }

  if (!m_bin) {
    qmlInfo(this) << "no playbin2";
    return false;
  }

  if (state == VideoPlayer::StatePaused) {
    m_timer->stop();

    // Set uri if needed:
    if (m_state == VideoPlayer::StateStopped) {
      const char *uri = m_url.toString().toUtf8().constData();
      g_object_set(m_bin, "uri", uri, NULL);
    }

    if (gst_element_set_state(m_bin, GST_STATE_PAUSED) == GST_STATE_CHANGE_FAILURE) {
      qmlInfo(this) << "error setting pipeline to PAUSED";
      return false;
    }

    GstState st;
    if (gst_element_get_state(m_bin, &st, NULL, GST_CLOCK_TIME_NONE)
	== GST_STATE_CHANGE_FAILURE) {
      qmlInfo(this) << "setting pipeline to PAUSED failed";
      return false;
    }

    if (st != GST_STATE_PAUSED) {
      qmlInfo(this) << "pipeline failed to transition to to PAUSED state";
      return false;
    }

    m_state = state;
    emit stateChanged();

    return true;
  }
  else if (state == VideoPlayer::StatePlaying) {
    // Set uri if needed:
    if (m_state == VideoPlayer::StateStopped) {
      const char *uri = m_url.toString().toUtf8().constData();
      g_object_set(m_bin, "uri", uri, NULL);
    }

    if (gst_element_set_state(m_bin, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
      qmlInfo(this) << "error setting pipeline to PLAYING";
      return false;
    }

    GstState st;
    if (gst_element_get_state(m_bin, &st, NULL, GST_CLOCK_TIME_NONE)
	== GST_STATE_CHANGE_FAILURE) {
      qmlInfo(this) << "setting pipeline to PLAYING failed";
      return false;
    }

    if (st != GST_STATE_PLAYING) {
      qmlInfo(this) << "pipeline failed to transition to to PLAYING state";
      return false;
    }

    m_state = state;
    emit stateChanged();

    emit durationChanged();
    emit positionChanged();

    m_timer->start();
    return true;
  }
  else {
    m_timer->stop();
    m_pos = 0;

    if (gst_element_set_state(m_bin, GST_STATE_NULL) == GST_STATE_CHANGE_FAILURE) {
      qmlInfo(this) << "error setting pipeline to NULL";
      return false;
    }

    GstState st;
    if (gst_element_get_state(m_bin, &st, NULL, GST_CLOCK_TIME_NONE)
	== GST_STATE_CHANGE_FAILURE) {
      qmlInfo(this) << "setting pipeline to NULL failed";
      return false;
    }

    if (st != GST_STATE_NULL) {
      qmlInfo(this) << "pipeline failed to transition to to NULL state";
      return false;
    }

    m_state = state;
    emit stateChanged();

    emit durationChanged();
    emit positionChanged();

    return true;
  }
}

gboolean VideoPlayer::bus_call(GstBus *bus, GstMessage *msg, gpointer data) {
  Q_UNUSED(bus);

  VideoPlayer *that = (VideoPlayer *) data;

  gchar *debug = NULL;
  GError *err = NULL;

  switch (GST_MESSAGE_TYPE(msg)) {
  case GST_MESSAGE_EOS:
    that->stop();
    break;

  case GST_MESSAGE_ERROR:
    gst_message_parse_error (msg, &err, &debug);

    emit that->error(err->message, err->code, debug);
    that->stop();
    if (err) {
      g_error_free (err);
    }

    if (debug) {
      g_free (debug);
    }

    break;

  default:
    break;
  }

  return TRUE;  
}
