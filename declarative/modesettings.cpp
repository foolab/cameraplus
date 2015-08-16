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

#include "modesettings.h"
#include "qtcammodesettings.h"
#include "camera.h"
#include "resolutionmodel.h"
#include "resolution.h"

ModeSettings::ModeSettings(QObject *parent) :
  QObject(parent),
  m_cam(0),
  m_settings(0),
  m_resolutions(0) {

}

ModeSettings::~ModeSettings() {
  setSettings(0);
}

QString ModeSettings::suffix() const {
  return m_settings ? m_settings->suffix() : QString();
}

QStringList ModeSettings::aspectRatios() const {
  return m_settings ? m_settings->aspectRatios() : QStringList();
}

Camera *ModeSettings::camera() {
  return m_cam;
}

void ModeSettings::setCamera(Camera *camera) {
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
}

void ModeSettings::deviceChanged() {
  resetSettings();

  emit settingsChanged();

  emit readyChanged();

  resolutionsUpdated();

  emit aspectRatioCountChanged();
}

void ModeSettings::prepareForDeviceChange() {
  setSettings(0);
}

ResolutionModel *ModeSettings::resolutions() {
  if (!m_settings) {
    return 0;
  }

  if (!m_resolutions) {
    m_resolutions = new ResolutionModel(m_settings, this);
  }

  return m_resolutions;
}

bool ModeSettings::isReady() const {
  return m_settings != 0;
}

bool ModeSettings::setResolution(const QString& resolution) {
  if (!isReady()) {
    return false;
  }

  QList<QtCamResolution> res = m_settings->resolutions();

  if (res.isEmpty()) {
    // store the resolution
    m_pendingResolution = resolution;
    return true;
  } else {
    m_pendingResolution.clear();
  }

  foreach (const QtCamResolution& r, res) {
    if (r.id() == resolution) {
      return setResolution(r);
    }
  }

  return false;
}

QString ModeSettings::aspectRatioForResolution(const QString& resolution) {
  if (!isReady()) {
    return QString();
  }

  QList<QtCamResolution> res = m_settings->resolutions();

  foreach (const QtCamResolution& r, res) {
    if (r.id() == resolution) {
      return r.aspectRatio();
    }
  }

  return QString();
}

QString ModeSettings::bestResolution(const QString& aspectRatio, const QString& resolution) {
  // TODO: We need to improve this or even remove it and find another way
  if (!isReady()) {
    return QString();
  }

  QList<QtCamResolution> res = m_settings->resolutions(aspectRatio);

  foreach (const QtCamResolution& r, res) {
    if (r.id() == resolution) {
      return resolution;
    }
  }

  if (!res.isEmpty()) {
    return res[0].id();
  }

  return QString();
}

bool ModeSettings::setViewfinderResolution(const QSize& resolution) {
  if (!isReady()) {
    return false;
  }

  if (!m_cam || !m_cam->device()) {
    return false;
  }

  QList<QtCamResolution> res = m_settings->resolutions();
  if (res.isEmpty()) {
    // Store the resolution:
    m_pendingViewfinderResolution = resolution;
    return true;
  } else {
    m_pendingViewfinderResolution = QSize();
  }

  foreach (const QtCamResolution& r, res) {
    if (r.allViewfinderResolutions().indexOf(resolution) != -1) {
      QtCamResolution newResolution(r);
      newResolution.setViewfinderResolution(resolution);

      return setResolution(newResolution);
    }
  }

  return false;
}

int ModeSettings::aspectRatioCount() const {
  return aspectRatios().count();
}

bool ModeSettings::setResolution(const QtCamResolution& resolution) {
  if (!isReady()) {
    return false;
  }

  if (!m_cam || !m_cam->device()) {
    return false;
  }

  return applyResolution(resolution);
}

void ModeSettings::setSettings(QtCamModeSettings *settings) {
  if (m_settings) {
    QObject::disconnect(m_settings, SIGNAL(resolutionsUpdated()),
			this, SLOT(resolutionsUpdated()));
  }

  m_settings = settings;

  if (m_settings) {
    QObject::connect(m_settings, SIGNAL(resolutionsUpdated()),
		     this, SLOT(resolutionsUpdated()));
  }
}

void ModeSettings::resolutionsUpdated() {
  delete m_resolutions;
  m_resolutions = 0;

  emit resolutionsChanged();

  if (!m_pendingResolution.isEmpty()) {
    // We need to copy the string because setResolution() will clear
    // m_pendingResolution. Since we pass a reference to m_pendingResolution,
    // clearing it will make the reference empty and will never match anything
    setResolution(QString(m_pendingResolution));
  } else if (m_pendingViewfinderResolution.isValid()) {
    setViewfinderResolution(QSize(m_pendingViewfinderResolution));
  }
}
