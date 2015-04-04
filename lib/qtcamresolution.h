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

#ifndef QT_CAM_RESOLUTION_H
#define QT_CAM_RESOLUTION_H

#include <QSharedDataPointer>
#include <QSize>
#include <QString>
#include <QVariant>
#include <QTextStream>

class QtCamResolutionPrivate;

class QtCamResolution {
public:
  typedef enum {
    ModeImage,
    ModeVideo,
  } Mode;

  QtCamResolution(const QString& id, const QString& aspectRatio,
		  const QSize& capture, const QSize& preview, const QSize& viewfinder,
		  int fps, int nightFps, int zslFps, float megaPixels, const QString& commonName,
		  const Mode& mode, const QVariant device);
  QtCamResolution(const QtCamResolution& other);
  QtCamResolution(const QtCamResolution::Mode& mode);

  QtCamResolution& operator=(const QtCamResolution& other);

  ~QtCamResolution();

  bool isValid() const;

  QString id() const;
  void setId(const QString& id);

  QString aspectRatio() const;
  void setAspectRatio(const QString& aspectRatio);

  QString commonName() const;
  void setCommonName(const QString& commonName);

  QSize captureResolution() const;
  void setCaptureResolution(const QSize& captureResolution);

  QSize viewfinderResolution() const;
  void setViewfinderResolution(const QSize& viewfinderResolution);

  QSize previewResolution() const;
  void setPreviewResolution(const QSize& previewResolution);

  int frameRate() const;
  void setFrameRate(int frameRate);

  int nightFrameRate() const;
  void setNightFrameRate(int nightFrameRate);

  int zslFrameRate() const;
  void setZslFrameRate(int zslFrameRate);

  float megaPixels() const;
  void setMegaPixels(float megaPixels);

  Mode mode() const;
  void setMode(const Mode& mode);

  QVariant device() const;
  void setDevice(const QVariant& device);

  friend QTextStream& operator<<(QTextStream& s, const QtCamResolution& res);

private:
  QSharedDataPointer<QtCamResolutionPrivate> d_ptr;
};

#endif /* QT_CAM_RESOLUTION_H */
