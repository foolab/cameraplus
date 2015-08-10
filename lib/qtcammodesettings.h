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

#ifndef QT_CAM_MODE_SETTINGS_H
#define QT_CAM_MODE_SETTINGS_H

#include <QObject>
#include <QSize>
#include <QPair>
#include <QString>
#include <QStringList>
#include "qtcamresolution.h"

class QtCamModeSettingsPrivate;

class QtCamModeSettings : public QObject {
  Q_OBJECT

public:
  QtCamModeSettings(const QString& id, const QString& suffix, const QString& profileName,
		    const QString& profilePath, const QList<QtCamResolution>& resolutions,
		    const QtCamResolution::Mode& mode, QObject *parent = 0);

  virtual ~QtCamModeSettings();

  QString id() const;
  QString suffix() const;
  QString profileName() const;
  QString profilePath() const;

  QtCamResolution defaultResolution(const QString& aspectRatio = QString()) const;
  QList<QtCamResolution> resolutions(const QString& aspectRatio = QString()) const;
  QList<QtCamResolution> hiddenResolutions() const;

  QStringList aspectRatios() const;

  bool hasResolutions() const;
  void updateResolutions(const QList<QtCamResolution>& resolutions);

signals:
  void resolutionsUpdated();

private:
  QtCamModeSettingsPrivate *d_ptr;
};

#endif /* QT_CAM_MODE_SETTINGS_H */
