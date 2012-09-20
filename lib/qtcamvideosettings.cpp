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

#include "qtcamvideosettings.h"

class QtCamVideoSettingsPrivate {
public:
  QString id;
  QString name;
  QSize capture;
  QSize preview;
  int fps;
  int nightFps;
};

QtCamVideoSettings::QtCamVideoSettings(const QString& id, const QString& name,
				       const QSize& capture, const QSize& preview,
				       int fps, int nightFps) :
  d_ptr(new QtCamVideoSettingsPrivate) {

  d_ptr->id = id;
  d_ptr->name = name;
  d_ptr->capture = capture;
  d_ptr->preview = preview;
  d_ptr->fps = fps;
  d_ptr->nightFps = nightFps;
}

QtCamVideoSettings::QtCamVideoSettings(const QtCamVideoSettings& other) :
  d_ptr(new QtCamVideoSettingsPrivate) {

  d_ptr->id = other.d_ptr->id;
  d_ptr->name = other.d_ptr->name;
  d_ptr->capture = other.d_ptr->capture;
  d_ptr->preview = other.d_ptr->preview;
  d_ptr->fps = other.d_ptr->fps;
  d_ptr->nightFps = other.d_ptr->nightFps;
}

QtCamVideoSettings::~QtCamVideoSettings() {
  delete d_ptr;
}

QtCamVideoSettings& QtCamVideoSettings::operator=(const QtCamVideoSettings&
								other) {
  d_ptr->id = other.d_ptr->id;
  d_ptr->name = other.d_ptr->name;
  d_ptr->capture = other.d_ptr->capture;
  d_ptr->preview = other.d_ptr->preview;
  d_ptr->fps = other.d_ptr->fps;
  d_ptr->nightFps = other.d_ptr->nightFps;

  return *this;
}

QString QtCamVideoSettings::id() const {
  return d_ptr->id;
}

QString QtCamVideoSettings::name() const {
  return d_ptr->name;
}

QSize QtCamVideoSettings::captureResolution() const {
  return d_ptr->capture;
}

QSize QtCamVideoSettings::previewResolution() const {
  return d_ptr->preview;
}

int QtCamVideoSettings::frameRate() const {
  return d_ptr->fps;
}

int QtCamVideoSettings::nightFrameRate() const {
  return d_ptr->nightFps;
}
