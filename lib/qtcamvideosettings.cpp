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

#include "qtcamvideosettings.h"

class QtCamVideoSettingsPrivate : public QSharedData {
public:
  QString id;
  QString suffix;
  QString profileName;
  QString profilePath;
  QList<QtCamResolution> resolutions;
};

QtCamVideoSettings::QtCamVideoSettings(const QString& id, const QString& suffix,
				       const QString& profileName,
				       const QString& profilePath,
				       const QList<QtCamResolution>& resolutions) :
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

QtCamResolution QtCamVideoSettings::defaultResolution(const QString& aspectRatio) const {
  if (d_ptr->resolutions.isEmpty()) {
    return QtCamResolution(QtCamResolution::ModeVideo);
  }

  if (aspectRatio.isEmpty()) {
    return d_ptr->resolutions[0];
  }

  foreach (const QtCamResolution& r, d_ptr->resolutions) {
    if (r.aspectRatio() == aspectRatio) {
      return r;
    }
  }

  return d_ptr->resolutions[0];
}

QList<QtCamResolution> QtCamVideoSettings::resolutions(const QString& aspectRatio) const {
  if (aspectRatio.isEmpty()) {
    return d_ptr->resolutions;
  }

  QList<QtCamResolution> res;

  foreach (const QtCamResolution& r, d_ptr->resolutions) {
    if (r.aspectRatio() == aspectRatio) {
      res << r;
    }
  }

  return res;
}

QStringList QtCamVideoSettings::aspectRatios() const {
  QStringList aspects;

  foreach (const QtCamResolution& r, d_ptr->resolutions) {
    if (aspects.indexOf(r.aspectRatio()) == -1) {
      aspects << r.aspectRatio();
    }
  }

  return aspects;
}

bool QtCamVideoSettings::hasResolutions() const {
  return !d_ptr->resolutions.isEmpty();
}

void QtCamVideoSettings::updateResolutions(const QList<QtCamResolution>& resolutions) {
  d_ptr->resolutions = resolutions;
}
