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

#include "qtcamimagesettings.h"
#include <QSharedData>
#include <QDebug>

class QtCamImageResolutionPrivate : public QSharedData {
public:
  QString id;
  QString name;
  QSize capture;
  QSize preview;
  QSize viewfinder;
  int fps;
  int nightFps;
  int megaPixels;
  QString aspectRatio;
};

class QtCamImageSettingsPrivate : public QSharedData {
public:
  QString id;
  QString suffix;
  QString profileName;
  QString profilePath;
  QList<QtCamImageResolution> resolutions;
};


QtCamImageResolution::QtCamImageResolution(const QString& id, const QString& name,
					   const QSize& capture, const QSize& preview,
					   const QSize& viewfinder, int fps, int nightFps,
					   int megaPixels, QString aspectRatio) :
  d_ptr(new QtCamImageResolutionPrivate) {
  d_ptr->id = id;
  d_ptr->name = name;
  d_ptr->capture = capture;
  d_ptr->preview = preview;
  d_ptr->viewfinder = viewfinder;
  d_ptr->fps = fps;
  d_ptr->nightFps = nightFps;
  d_ptr->megaPixels = megaPixels;
  d_ptr->aspectRatio = aspectRatio;
}

QtCamImageResolution::QtCamImageResolution(const QtCamImageResolution& other) :
  d_ptr(other.d_ptr) {

}

QtCamImageResolution& QtCamImageResolution::operator=(const QtCamImageResolution& other) {
  d_ptr = other.d_ptr;

  return *this;
}

QtCamImageResolution::~QtCamImageResolution() {
  // QSharedData will take care of reference counting.
}

QString QtCamImageResolution::id() const {
  return d_ptr->id;
}

QString QtCamImageResolution::name() const {
  return d_ptr->name;
}

QSize QtCamImageResolution::captureResolution() const {
  return d_ptr->capture;
}

QSize QtCamImageResolution::viewfinderResolution() const {
  return d_ptr->viewfinder;
}

QSize QtCamImageResolution::previewResolution() const {
  return d_ptr->preview;
}

int QtCamImageResolution::frameRate() const {
  return d_ptr->fps;
}

int QtCamImageResolution::nightFrameRate() const {
  return d_ptr->nightFps;
}

int QtCamImageResolution::megaPixels() const {
  return d_ptr->megaPixels;
}

QString QtCamImageResolution::aspectRatio() const {
  return d_ptr->aspectRatio;
}

QtCamImageSettings::QtCamImageSettings(const QString& id, const QString& suffix,
				       const QString& profileName, const QString& profilePath,
				       const QList<QtCamImageResolution>& resolutions) :
  d_ptr(new QtCamImageSettingsPrivate) {

  d_ptr->id = id;
  d_ptr->suffix = suffix;
  d_ptr->profileName = profileName;
  d_ptr->profilePath = profilePath;
  d_ptr->resolutions = resolutions;
}

QtCamImageSettings::QtCamImageSettings(const QtCamImageSettings& other) :
  d_ptr(other.d_ptr) {

}

QtCamImageSettings& QtCamImageSettings::operator=(const QtCamImageSettings& other) {
  d_ptr = other.d_ptr;

  return *this;
}

QtCamImageSettings::~QtCamImageSettings() {
  // QSharedData will take care of reference counting.
}

QString QtCamImageSettings::id() const {
  return d_ptr->id;
}

QString QtCamImageSettings::suffix() const {
  return d_ptr->suffix;
}

QString QtCamImageSettings::profileName() const {
  return d_ptr->profileName;
}

QString QtCamImageSettings::profilePath() const {
  return d_ptr->profilePath;
}

QtCamImageResolution QtCamImageSettings::defaultResolution(const QString& aspectRatio) const {
  if (aspectRatio.isEmpty()) {
    return d_ptr->resolutions[0];
  }

  foreach (const QtCamImageResolution& r, d_ptr->resolutions) {
    if (r.aspectRatio() == aspectRatio) {
      return r;
    }
  }

  return d_ptr->resolutions[0];
}

QList<QtCamImageResolution> QtCamImageSettings::resolutions(const QString& aspectRatio) const {
  if (aspectRatio.isEmpty()) {
    return d_ptr->resolutions;
  }

  QList<QtCamImageResolution> res;

  foreach (const QtCamImageResolution& r, d_ptr->resolutions) {
    if (r.aspectRatio() == aspectRatio) {
      res << r;
    }
  }

  return res;
}

QStringList QtCamImageSettings::aspectRatios() const {
  QStringList aspects;

  foreach (const QtCamImageResolution& r, d_ptr->resolutions) {
    if (aspects.indexOf(r.aspectRatio()) == -1) {
      aspects << r.aspectRatio();
    }
  }

  return aspects;
}
