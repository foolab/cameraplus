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

#include "qtcamvideosettings.h"

class QtCamVideoResolutionPrivate : public QSharedData {
public:
  QString id;
  QString name;
  QSize capture;
  QSize preview;
  int fps;
  int nightFps;
  QString aspectRatio;
  QString resolution;
};

class QtCamVideoSettingsPrivate : public QSharedData {
public:
  QString id;
  QString suffix;
  QString profileName;
  QString profilePath;
  QList<QtCamVideoResolution> resolutions;
};

QtCamVideoResolution::QtCamVideoResolution(const QString& id, const QString& name,
					   const QSize& capture, const QSize& preview,
					   int fps, int nightFps,
					   const QString& aspectRatio,
					   const QString& resolution) :
  d_ptr(new QtCamVideoResolutionPrivate) {
  d_ptr->id = id;
  d_ptr->name = name;
  d_ptr->capture = capture;
  d_ptr->preview = preview;
  d_ptr->fps = fps;
  d_ptr->nightFps = nightFps;
  d_ptr->aspectRatio = aspectRatio;
  d_ptr->resolution = resolution;
}

QtCamVideoResolution::QtCamVideoResolution(const QtCamVideoResolution& other) :
  d_ptr(other.d_ptr) {

}

QtCamVideoResolution& QtCamVideoResolution::operator=(const QtCamVideoResolution& other) {
  d_ptr = other.d_ptr;

  return *this;
}

QtCamVideoResolution::~QtCamVideoResolution() {
  // QSharedData will take care of reference counting.
}

QString QtCamVideoResolution::id() const {
  return d_ptr->id;
}

QString QtCamVideoResolution::name() const {
  return d_ptr->name;
}

QSize QtCamVideoResolution::captureResolution() const {
  return d_ptr->capture;
}

QSize QtCamVideoResolution::previewResolution() const {
  return d_ptr->preview;
}

int QtCamVideoResolution::frameRate() const {
  return d_ptr->fps;
}

int QtCamVideoResolution::nightFrameRate() const {
  return d_ptr->nightFps;
}

QString QtCamVideoResolution::aspectRatio() const {
  return d_ptr->aspectRatio;
}

QString QtCamVideoResolution::resolution() const {
  return d_ptr->resolution;
}

QtCamVideoSettings::QtCamVideoSettings(const QString& id, const QString& suffix,
				       const QString& profileName,
				       const QString& profilePath,
				       const QList<QtCamVideoResolution>& resolutions) :
  d_ptr(new QtCamVideoSettingsPrivate) {

  d_ptr->id = id;
  d_ptr->suffix = suffix;
  d_ptr->profileName = profileName;
  d_ptr->profilePath = profilePath;
  d_ptr->resolutions = resolutions;
}

QtCamVideoSettings::QtCamVideoSettings(const QtCamVideoSettings& other) :
  d_ptr(other.d_ptr) {

}

QtCamVideoSettings& QtCamVideoSettings::operator=(const QtCamVideoSettings& other) {
  d_ptr = other.d_ptr;

  return *this;
}

QtCamVideoSettings::~QtCamVideoSettings() {
  // QSharedData will take care of reference counting.
}

QString QtCamVideoSettings::id() const {
  return d_ptr->id;
}

QString QtCamVideoSettings::suffix() const {
  return d_ptr->suffix;
}

QString QtCamVideoSettings::profileName() const {
  return d_ptr->profileName;
}

QString QtCamVideoSettings::profilePath() const {
  return d_ptr->profilePath;
}

QtCamVideoResolution QtCamVideoSettings::defaultResolution(const QString& aspectRatio) const {
  if (aspectRatio.isEmpty()) {
    return d_ptr->resolutions[0];
  }

  foreach (const QtCamVideoResolution& r, d_ptr->resolutions) {
    if (r.aspectRatio() == aspectRatio) {
      return r;
    }
  }

  return d_ptr->resolutions[0];
}

QList<QtCamVideoResolution> QtCamVideoSettings::resolutions(const QString& aspectRatio) const {
  if (aspectRatio.isEmpty()) {
    return d_ptr->resolutions;
  }

  QList<QtCamVideoResolution> res;

  foreach (const QtCamVideoResolution& r, d_ptr->resolutions) {
    if (r.aspectRatio() == aspectRatio) {
      res << r;
    }
  }

  return res;
}

QStringList QtCamVideoSettings::aspectRatios() const {
  QStringList aspects;

  foreach (const QtCamVideoResolution& r, d_ptr->resolutions) {
    if (aspects.indexOf(r.aspectRatio()) == -1) {
      aspects << r.aspectRatio();
    }
  }

  return aspects;
}
