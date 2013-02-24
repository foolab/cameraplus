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

#ifndef QT_CAM_IMAGE_SETTINGS_H
#define QT_CAM_IMAGE_SETTINGS_H

#include <QSize>
#include <QPair>
#include <QString>
#include <QSharedDataPointer>
#include <QStringList>

class QtCamImageSettingsPrivate;
class QtCamImageResolutionPrivate;

class QtCamImageResolution {
public:
  QtCamImageResolution(const QString& id, const QString& name, const QSize& capture,
		       const QSize& preview, const QSize& viewfinder,
		       int fps, int nightFps, int megaPixels, QString aspectRatio);
  QtCamImageResolution(const QtCamImageResolution& other);

  QtCamImageResolution& operator=(const QtCamImageResolution& other);

  ~QtCamImageResolution();

  QString id() const;
  QString name() const;
  QSize captureResolution() const;
  QSize viewfinderResolution() const;
  QSize previewResolution() const;
  int frameRate() const;
  int nightFrameRate() const;
  int megaPixels() const;
  QString aspectRatio() const;

private:
  QSharedDataPointer<QtCamImageResolutionPrivate> d_ptr;
};

class QtCamImageSettings {
public:
  QtCamImageSettings(const QString& id, const QString& suffix, const QString& profileName,
		     const QString& profilePath, const QList<QtCamImageResolution>& resolutions);
  QtCamImageSettings(const QtCamImageSettings& other);

  QtCamImageSettings& operator=(const QtCamImageSettings& other);

  ~QtCamImageSettings();

  QString id() const;
  QString suffix() const;
  QString profileName() const;
  QString profilePath() const;

  QtCamImageResolution defaultResolution(const QString& aspectRatio = QString()) const;
  QList<QtCamImageResolution> resolutions(const QString& aspectRatio = QString()) const;
  QStringList aspectRatios() const;

private:
  QSharedDataPointer<QtCamImageSettingsPrivate> d_ptr;
};

#endif /* QT_CAM_IMAGE_SETTINGS_H */
