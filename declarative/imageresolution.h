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

#ifndef IMAGE_RESOLUTION_H
#define IMAGE_RESOLUTION_H

#include <QObject>
#include "qtcamimagesettings.h"

class ImageResolution : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString resolutionLd READ resolutionId WRITE setResolutionId NOTIFY resolutionIdChanged);
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged);
  Q_PROPERTY(QString aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY aspectRatioChanged);
  Q_PROPERTY(QSize captureResolution READ capture WRITE setCapture NOTIFY captureChanged);
  Q_PROPERTY(QSize previewResolution READ preview WRITE setPreview NOTIFY previewChanged);
  Q_PROPERTY(QSize viewfinderResolution READ viewfinder WRITE setViewfinder NOTIFY viewfinderChanged);
  Q_PROPERTY(int frameRate READ fps WRITE setFps NOTIFY fpsChanged);
  Q_PROPERTY(int nightFrameRate READ nightFps WRITE setNightFps NOTIFY nightFpsChanged);
  Q_PROPERTY(int megaPixels READ megaPixels WRITE setMegaPixels NOTIFY megaPixelsChanged);

public:
  ImageResolution(QObject *parent = 0);
  ImageResolution(const QtCamImageResolution& resolution, QObject *parent = 0);
  ~ImageResolution();

  QtCamImageResolution resolution();

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

  QSize viewfinder() const;
  void setViewfinder(const QSize& viewfinder);

  int fps() const;
  void setFps(int fps);

  int nightFps() const;
  void setNightFps(int nightFps);

  int megaPixels() const;
  void setMegaPixels(int megaPixels);

signals:
  void resolutionIdChanged();
  void nameChanged();
  void captureChanged();
  void previewChanged();
  void viewfinderChanged();
  void fpsChanged();
  void nightFpsChanged();
  void megaPixelsChanged();
  void aspectRatioChanged();

private:
  QString m_resolutionId;
  QString m_name;
  QString m_aspectRatio;
  QSize m_capture;
  QSize m_preview;
  QSize m_viewfinder;
  int m_fps;
  int m_nightFps;
  int m_megaPixels;
};

#endif /* IMAGE_RESOLUTION_H */
