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

#ifndef SOUNDS_H
#define SOUNDS_H

#include <QObject>
#include "notifications.h"
#include <canberra.h>

class QtCamConfig;

class Sounds : public QObject, public Notifications {
  Q_OBJECT
  Q_INTERFACES(Notifications);

  Q_PROPERTY(bool mute READ isMuted WRITE setMuted NOTIFY muteChanged);

public:
  Sounds(QObject *parent = 0);
  ~Sounds();

  void imageCaptureStarted();
  void imageCaptureEnded();
  void videoRecordingStarted();
  void videoRecordingEnded();
  void autoFocusAcquired();

  bool isMuted() const;
  void setMuted(bool mute);

  void setConfig(QtCamConfig *conf);
  void reload();

signals:
  void muteChanged();

private:
  void cache(const QString& path, const char *id);
  void play(const char *id);
  void playAndBlock(const char *id);

  bool m_muted;
  ca_context *m_ctx;
  QtCamConfig *m_conf;
};

#endif /* SOUNDS_H */
