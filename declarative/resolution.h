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

#ifndef RESOLUTION_H
#define RESOLUTION_H

#include <QObject>
#include "qtcamimagesettings.h"

class Resolution : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString resolutionLd READ resolutionId WRITE setResolutionId NOTIFY resolutionIdChanged);
  Q_PROPERTY(QString aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY aspectRatioChanged);
  Q_PROPERTY(QString commonName READ commonName WRITE setCommonName NOTIFY commonNameChanged);
  Q_PROPERTY(QSize captureResolution READ capture WRITE setCapture NOTIFY captureChanged);
  Q_PROPERTY(QSize previewResolution READ preview WRITE setPreview NOTIFY previewChanged);
  Q_PROPERTY(QSize viewfinderResolution READ viewfinder WRITE setViewfinder NOTIFY viewfinderChanged);
  Q_PROPERTY(int frameRate READ fps WRITE setFps NOTIFY fpsChanged);
  Q_PROPERTY(int nightFrameRate READ nightFps WRITE setNightFps NOTIFY nightFpsChanged);
  Q_PROPERTY(float megaPixels READ megaPixels WRITE setMegaPixels NOTIFY megaPixelsChanged);

public:
  Resolution(QObject *parent = 0);
  Resolution(const QtCamResolution& resolution, QObject *parent = 0);
  ~Resolution();

  QtCamResolution resolution();

  QString resolutionId() const;
  void setResolutionId(const QString& resolutionId);

  QString aspectRatio() const;
  void setAspectRatio(const QString& aspectRatio);

  QString commonName() const;
  void setCommonName(const QString& commonName);

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

  float megaPixels() const;
  void setMegaPixels(float megaPixels);

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
  void commonNameChanged();

private:
  QtCamResolution m_resolution;
};

#endif /* RESOLUTION_H */
