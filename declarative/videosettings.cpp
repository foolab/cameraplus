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

#include "videosettings.h"
#include "qtcamvideosettings.h"
#include "camera.h"
#include "qtcamdevice.h"
#include "qtcamvideomode.h"
#include "videoresolutionmodel.h"
#include "videoresolution.h"
#include <QDebug>

VideoSettings::VideoSettings(QObject *parent) :
  QObject(parent),
  m_cam(0),
  m_settings(0),
  m_resolutions(0),
  m_currentResolution(0) {

}

VideoSettings::~VideoSettings() {
  m_settings = 0;

  if (m_currentResolution) {
    delete m_currentResolution;
    m_currentResolution = 0;
  }
}

QString VideoSettings::suffix() const {
  return m_settings ? m_settings->suffix() : QString();
}

QStringList VideoSettings::aspectRatios() const {
  return m_settings ? m_settings->aspectRatios() : QStringList();
}

Camera *VideoSettings::camera() {
  return m_cam;
}

void VideoSettings::setCamera(Camera *camera) {
  if (camera == m_cam) {
    return;
  }

  if (m_cam) {
    QObject::disconnect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
    QObject::disconnect(m_cam, SIGNAL(prepareForDeviceChange()),
			this, SLOT(prepareForDeviceChange()));
  }

  m_cam = camera;

  if (m_cam) {
    QObject::connect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
    QObject::connect(m_cam, SIGNAL(prepareForDeviceChange()),
		     this, SLOT(prepareForDeviceChange()));
  }

  emit cameraChanged();

  if (m_cam->device()) {
    deviceChanged();
  }
  else {
    delete m_currentResolution;
    m_currentResolution = 0;
    emit currentResolutionChanged();
  }
}

void VideoSettings::deviceChanged() {
  m_settings = m_cam->device()->videoMode()->settings();

  emit settingsChanged();

  emit readyChanged();

  delete m_resolutions;
  m_resolutions = 0;

  delete m_currentResolution;
  m_currentResolution = 0;

  emit aspectRatioCountChanged();
  emit resolutionsChanged();
  emit currentResolutionChanged();
}

void VideoSettings::prepareForDeviceChange() {
  m_settings = 0;
}

VideoResolutionModel *VideoSettings::resolutions() {
  if (!m_settings) {
    return 0;
  }

  if (!m_resolutions) {
    m_resolutions = new VideoResolutionModel(m_settings, this);
  }

  return m_resolutions;
}

bool VideoSettings::isReady() const {
  return m_settings != 0;
}

bool VideoSettings::setResolution(const QString& aspectRatio, const QString& resolution) {
  if (!isReady()) {
    return false;
  }

  QList<QtCamVideoResolution> res = m_settings->resolutions(aspectRatio);

  foreach (const QtCamVideoResolution& r, res) {
    if (r.name() == resolution) {
      return setResolution(r);
    }
  }

  return false;
}

int VideoSettings::aspectRatioCount() const {
  return aspectRatios().count();
}

VideoResolution *VideoSettings::currentResolution() {
  if (m_currentResolution) {
    return m_currentResolution;
  }

  if (!m_cam || !m_cam->device()) {
    return 0;
  }

  m_currentResolution = new VideoResolution(m_cam->device()->videoMode()->currentResolution());

  return m_currentResolution;
}

VideoResolution *VideoSettings::findResolution(const QString& aspectRatio,
					       const QString& name) {
  if (!isReady()) {
    return 0;
  }

  QList<QtCamVideoResolution> res = m_settings->resolutions(aspectRatio);

  foreach (const QtCamVideoResolution& r, res) {
    if (r.name() == name) {
      return new VideoResolution(r);
    }
  }

  return 0;
}

bool VideoSettings::setResolution(VideoResolution *resolution) {
  return setResolution(resolution->resolution());
}

bool VideoSettings::setResolution(const QtCamVideoResolution& resolution) {
  if (!isReady()) {
    return false;
  }

  if (!m_cam || !m_cam->device()) {
    return false;
  }

  if (m_cam->device()->videoMode()->setResolution(resolution)) {
    delete m_currentResolution;
    m_currentResolution = 0;

    emit currentResolutionChanged();

    return true;
  }

  return false;
}
