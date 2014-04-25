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

#include "imagesettings.h"
#include "qtcamimagesettings.h"
#include "camera.h"
#include "qtcamdevice.h"
#include "qtcamimagemode.h"
#include "imageresolutionmodel.h"
#include "imageresolution.h"
#include <QDebug>

ImageSettings::ImageSettings(QObject *parent) :
  QObject(parent),
  m_cam(0),
  m_settings(0),
  m_resolutions(0),
  m_currentResolution(0) {

}

ImageSettings::~ImageSettings() {
  m_settings = 0;

  if (m_currentResolution) {
    delete m_currentResolution;
    m_currentResolution = 0;
  }
}

QString ImageSettings::suffix() const {
  return m_settings ? m_settings->suffix() : QString();
}

QStringList ImageSettings::aspectRatios() const {
  return m_settings ? m_settings->aspectRatios() : QStringList();
}

Camera *ImageSettings::camera() {
  return m_cam;
}

void ImageSettings::setCamera(Camera *camera) {
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

void ImageSettings::deviceChanged() {
  m_settings = m_cam->device()->imageMode()->settings();

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

void ImageSettings::prepareForDeviceChange() {
  m_settings = 0;
}

ImageResolutionModel *ImageSettings::resolutions() {
  if (!m_settings) {
    return 0;
  }

  if (!m_resolutions) {
    m_resolutions = new ImageResolutionModel(m_settings, this);
  }

  return m_resolutions;
}

bool ImageSettings::isReady() const {
  return m_settings != 0;
}

bool ImageSettings::setResolution(const QString& aspectRatio, const QString& resolution) {
  if (!isReady()) {
    return false;
  }

  QList<QtCamImageResolution> res = m_settings->resolutions(aspectRatio);

  foreach (const QtCamImageResolution& r, res) {
    if (r.name() == resolution) {
      return setResolution(r);
    }
  }

  return false;
}

int ImageSettings::aspectRatioCount() const {
  return aspectRatios().count();
}

ImageResolution *ImageSettings::currentResolution() {
  if (m_currentResolution) {
    return m_currentResolution;
  }

  if (!m_cam || !m_cam->device()) {
    return 0;
  }

  m_currentResolution = new ImageResolution(m_cam->device()->imageMode()->currentResolution());

  return m_currentResolution;
}

ImageResolution *ImageSettings::findResolution(const QString& aspectRatio,
					       const QString& name) {
  if (!isReady()) {
    return 0;
  }

  QList<QtCamImageResolution> res = m_settings->resolutions(aspectRatio);

  foreach (const QtCamImageResolution& r, res) {
    if (r.name() == name) {
      return new ImageResolution(r);
    }
  }

  return 0;
}

bool ImageSettings::setResolution(ImageResolution *resolution) {
  return setResolution(resolution->resolution());
}

bool ImageSettings::setResolution(const QtCamImageResolution& resolution) {
  if (!isReady()) {
    return false;
  }

  if (!m_cam || !m_cam->device()) {
    return false;
  }

  if (m_cam->device()->imageMode()->setResolution(resolution)) {
    delete m_currentResolution;
    m_currentResolution = 0;

    emit currentResolutionChanged();

    return true;
  }

  return false;
}

QString ImageSettings::bestResolution(const QString& aspectRatio, const QString& resolution) {
  if (!isReady()) {
    return QString();
  }

  QList<QtCamImageResolution> res = m_settings->resolutions(aspectRatio);

  foreach (const QtCamImageResolution& r, res) {
    if (r.name() == resolution) {
      return resolution;
    }
  }

  if (!res.isEmpty()) {
    return res[0].name();
  }

  return QString();
}
