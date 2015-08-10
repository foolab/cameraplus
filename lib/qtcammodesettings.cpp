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

#include "qtcammodesettings.h"

class QtCamModeSettingsPrivate {
public:
  QtCamResolution::Mode mode;
  QString id;
  QString suffix;
  QString profileName;
  QString profilePath;
  QList<QtCamResolution> resolutions;
  QList<QtCamResolution> hiddenResolutions;
};

QtCamModeSettings::QtCamModeSettings(const QString& id, const QString& suffix,
				     const QString& profileName, const QString& profilePath,
				     const QList<QtCamResolution>& resolutions,
				     const QtCamResolution::Mode& mode, QObject *parent) :
  QObject(parent),
  d_ptr(new QtCamModeSettingsPrivate) {

  d_ptr->mode = mode;
  d_ptr->id = id;
  d_ptr->suffix = suffix;
  d_ptr->profileName = profileName;
  d_ptr->profilePath = profilePath;

  foreach (const QtCamResolution& r, resolutions) {
    if (r.isVisible()) {
      d_ptr->resolutions << r;
    } else {
      d_ptr->hiddenResolutions << r;
    }
  }
}

QtCamModeSettings::~QtCamModeSettings() {
  delete d_ptr; d_ptr = 0;
}

QString QtCamModeSettings::id() const {
  return d_ptr->id;
}

QString QtCamModeSettings::suffix() const {
  return d_ptr->suffix;
}

QString QtCamModeSettings::profileName() const {
  return d_ptr->profileName;
}

QString QtCamModeSettings::profilePath() const {
  return d_ptr->profilePath;
}

QtCamResolution QtCamModeSettings::defaultResolution(const QString& aspectRatio) const {
  if (d_ptr->resolutions.isEmpty()) {
    return QtCamResolution(d_ptr->mode);
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

QList<QtCamResolution> QtCamModeSettings::resolutions(const QString& aspectRatio) const {
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

QList<QtCamResolution> QtCamModeSettings::hiddenResolutions() const {
  return d_ptr->hiddenResolutions;
}

QStringList QtCamModeSettings::aspectRatios() const {
  QStringList aspects;

  foreach (const QtCamResolution& r, d_ptr->resolutions) {
    if (aspects.indexOf(r.aspectRatio()) == -1) {
      aspects << r.aspectRatio();
    }
  }

  return aspects;
}

bool QtCamModeSettings::hasResolutions() const {
  return !d_ptr->resolutions.isEmpty();
}

void QtCamModeSettings::updateResolutions(const QList<QtCamResolution>& resolutions) {
  d_ptr->resolutions = resolutions;

  emit resolutionsUpdated();
}
