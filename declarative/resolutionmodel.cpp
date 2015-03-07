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

#include "resolutionmodel.h"
#include "qtcamimagesettings.h"
#include "qtcamvideosettings.h"
#include <QDebug>

ResolutionModel::ResolutionModel(QtCamImageSettings *settings, QObject *parent) :
  QAbstractListModel(parent),
  m_image(settings),
  m_video(0) {

  init();

  m_resolutions = m_image->resolutions(m_aspectRatio);
}

ResolutionModel::ResolutionModel(QtCamVideoSettings *settings, QObject *parent) :
  QAbstractListModel(parent),
  m_image(0),
  m_video(settings) {

  init();

  m_resolutions = m_video->resolutions(m_aspectRatio);
}

ResolutionModel::~ResolutionModel() {
  m_image = 0;
  m_video = 0;
}

void ResolutionModel::init() {
  QHash<int, QByteArray> roles;
  roles[IdRole] = "resolutionId";
  roles[CaptureRole] = "captureResolution";
  roles[PreviewRole] = "previewResolution";
  roles[FpsRole] = "frameRate";
  roles[NightFpsRole] = "nightFrameRate";
  roles[ResolutionRole] = "resolution";
  roles[MegaPixelsRole] = "megaPixels";
  roles[AspectRatioRole] = "resolutionAspectRatio";

  setRoleNames(roles);
}

int ResolutionModel::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid()) {
    return m_resolutions.size();
  }

  return 0;
}

QVariant ResolutionModel::data(const QModelIndex& index, int role) const {
  if (index.row() < 0 || index.row() >= m_resolutions.size()) {
    return QVariant();
  }

  const QtCamResolution& res = m_resolutions[index.row()];

  switch (role) {
  case IdRole:
    return res.id();

  case CaptureRole:
    return res.captureResolution();

  case PreviewRole:
    return res.previewResolution();

  case FpsRole:
    return res.frameRate();

  case NightFpsRole:
    return res.nightFrameRate();

  case MegaPixelsRole:
    return res.megaPixels();

  case AspectRatioRole:
    return res.aspectRatio();

  case ResolutionRole:
    return res.commonName();

  default:
    return QVariant();
  }
}

QString ResolutionModel::aspectRatio() const {
  return m_aspectRatio;
}

void ResolutionModel::setAspectRatio(const QString& aspectRatio) {
  if (aspectRatio != m_aspectRatio) {

    m_aspectRatio = aspectRatio;

    beginResetModel();

    // TODO: there is a crash here when we switch devices
    if (m_image) {
      m_resolutions = m_image->resolutions(m_aspectRatio);
    }
    else if (m_video) {
      m_resolutions = m_video->resolutions(m_aspectRatio);
    }

    endResetModel();

    emit aspectRatioChanged();
    emit countChanged();
  }
}

int ResolutionModel::count() const {
  return rowCount();
}

#if defined(QT5)
QHash<int, QByteArray> ResolutionModel::roleNames() const {
  return m_roles;
}

void ResolutionModel::setRoleNames(const QHash<int, QByteArray>& roles) {
  m_roles = roles;
}
#endif
