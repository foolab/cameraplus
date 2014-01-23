// -*- c++ -*-

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

#ifndef SOUNDS_H
#define SOUNDS_H

#include <QObject>
#include <canberra.h>

class QDBusServiceWatcher;

class Sounds : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool mute READ isMuted WRITE setMuted NOTIFY muteChanged);
  Q_PROPERTY(QString imageCaptureStart READ imageCaptureStart WRITE setImageCaptureStart NOTIFY imageCaptureStartChanged);
  Q_PROPERTY(QString imageCaptureEnd READ imageCaptureEnd WRITE setImageCaptureEnd NOTIFY imageCaptureEndChanged);
  Q_PROPERTY(QString videoRecordingStart READ videoRecordingStart WRITE setVideoRecordingStart NOTIFY videoRecordingStartChanged);
  Q_PROPERTY(QString videoRecordingEnd READ videoRecordingEnd WRITE setVideoRecordingEnd NOTIFY videoRecordingEndChanged);
  Q_PROPERTY(QString autoFocusAcquired READ autoFocusAcquired WRITE setAutoFocusAcquired NOTIFY autoFocusAcquiredChanged);
  Q_PROPERTY(Volume volume READ volume WRITE setVolume NOTIFY volumeChanged);
  Q_ENUMS(Volume);

public:
  Sounds(QObject *parent = 0);
  ~Sounds();

  typedef enum {
    VolumeLow,
    VolumeHigh,
  } Volume;

  void playImageCaptureStartedSound();
  void playImageCaptureEndedSound();
  void playVideoRecordingStartedSound();
  void playVideoRecordingEndedSound();
  void playAutoFocusAcquiredSound();

  bool isMuted() const;
  void setMuted(bool mute);

  Volume volume() const;
  void setVolume(const Volume& volume);

  void reload();

  QString imageCaptureStart() const;
  void setImageCaptureStart(const QString& path);

  QString imageCaptureEnd() const;
  void setImageCaptureEnd(const QString& path);

  QString videoRecordingStart() const;
  void setVideoRecordingStart(const QString& path);

  QString videoRecordingEnd() const;
  void setVideoRecordingEnd(const QString& path);

  QString autoFocusAcquired() const;
  void setAutoFocusAcquired(const QString& path);

signals:
  void muteChanged();
  void volumeChanged();
  void imageCaptureStartChanged();
  void imageCaptureEndChanged();
  void videoRecordingStartChanged();
  void videoRecordingEndChanged();
  void autoFocusAcquiredChanged();

private slots:
  void serviceOwnerChanged(const QString& serviceName, const QString& oldOwner,
			   const QString& newOwner);

private:
  void cache(const QString& path, const char *id);
  void play(const char *id);
  void playAndBlock(const char *id);

  bool m_muted;
  ca_context *m_ctx;
  Volume m_volume;
  QDBusServiceWatcher *m_watcher;
  QString m_volumeString;
  QString m_imageCaptureStart;
  QString m_imageCaptureEnd;
  QString m_videoRecordingStart;
  QString m_videoRecordingEnd;
  QString m_autoFocusAcquired;
};

#endif /* SOUNDS_H */
