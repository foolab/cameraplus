// -*- c++ -*-

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

#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <QDeclarativeItem>
#include <gst/gst.h>

class CameraConfig;
class QtCamGraphicsViewfinder;

class VideoPlayer : public QDeclarativeItem {
  Q_OBJECT

  Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged);
  Q_PROPERTY(CameraConfig *cameraConfig READ cameraConfig WRITE setCameraConfig NOTIFY cameraConfigChanged);
  Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged);
  Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged);
  Q_PROPERTY(State state READ state NOTIFY stateChanged);
  Q_ENUMS(State);

public:
  VideoPlayer(QDeclarativeItem *parent = 0);
  ~VideoPlayer();

  virtual void componentComplete();
  virtual void classBegin();

  QUrl source() const;
  void setSource(const QUrl& source);

  CameraConfig *cameraConfig() const;
  void setCameraConfig(CameraConfig *config);

  qint64 duration() const;
  qint64 position();
  void setPosition(qint64 position);

  Q_INVOKABLE bool pause();
  Q_INVOKABLE bool play();
  Q_INVOKABLE bool seek(qint64 offset);
  Q_INVOKABLE bool stop();

  typedef enum {
    StateStopped,
    StatePaused,
    StatePlaying,
  } State;

  State state() const;

signals:
  void sourceChanged();
  void cameraConfigChanged();

  void durationChanged();
  void positionChanged();
  void error(const QString& message, int code, const QString& debug);
  void stateChanged();

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

private:
  static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data);

  bool setState(const State& state);

  CameraConfig *m_config;
  QtCamGraphicsViewfinder *m_vf;
  QUrl m_url;

  GstElement *m_bin;
  State m_state;
  QTimer *m_timer;
  qint64 m_pos;
};

#endif /* VIDEO_PLAYER_H */
