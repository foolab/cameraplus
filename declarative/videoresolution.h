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

#ifndef VIDEO_RESOLUTION_H
#define VIDEO_RESOLUTION_H

#include <QObject>
#include "qtcamvideosettings.h"

class VideoResolution : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString resolutionLd READ resolutionId WRITE setResolutionId NOTIFY resolutionIdChanged);
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged);
  Q_PROPERTY(QString aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY aspectRatioChanged);
  Q_PROPERTY(QSize captureResolution READ capture WRITE setCapture NOTIFY captureChanged);
  Q_PROPERTY(QSize previewResolution READ preview WRITE setPreview NOTIFY previewChanged);
  Q_PROPERTY(int frameRate READ fps WRITE setFps NOTIFY fpsChanged);
  Q_PROPERTY(int nightFrameRate READ nightFps WRITE setNightFps NOTIFY nightFpsChanged);
  Q_PROPERTY(QString commonName READ commonName WRITE setCommonName NOTIFY commonNameChanged);

public:
  VideoResolution(QObject *parent = 0);
  VideoResolution(const QtCamVideoResolution& resolution, QObject *parent = 0);
  ~VideoResolution();

  QtCamVideoResolution resolution();

  QString resolutionId() const;
  void setResolutionId(const QString& resolutionId);

  QString name() const;
  void setName(const QString& name);

  QString aspectRatio() const;
  void setAspectRatio(const QString& aspectRatio);

  QSize capture() const;
  void setCapture(const QSize& capture);

  QSize preview() const;
  void setPreview(const QSize& preview);

  int fps() const;
  void setFps(int fps);

  int nightFps() const;
  void setNightFps(int nightFps);

  QString commonName() const;
  void setCommonName(const QString& commonName);

signals:
  void resolutionIdChanged();
  void nameChanged();
  void captureChanged();
  void previewChanged();
  void fpsChanged();
  void nightFpsChanged();
  void aspectRatioChanged();
  void commonNameChanged();

private:
  QString m_resolutionId;
  QString m_name;
  QString m_aspectRatio;
  QString m_commonName;
  QSize m_capture;
  QSize m_preview;
  int m_fps;
  int m_nightFps;
};

#endif /* VIDEO_RESOLUTION_H */
