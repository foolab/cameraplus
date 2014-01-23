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

#include "videoresolutionmodel.h"
#include "qtcamvideosettings.h"
#include <QDebug>

VideoResolutionModel::VideoResolutionModel(QtCamVideoSettings *settings, QObject *parent) :
  QAbstractListModel(parent), m_settings(settings) {

  QHash<int, QByteArray> roles;
  roles[IdRole] = "resolutionId";
  roles[NameRole] = "resolutionName";
  roles[CaptureRole] = "captureResolution";
  roles[PreviewRole] = "previewResolution";
  roles[FpsRole] = "frameRate";
  roles[NightFpsRole] = "nightFrameRate";
  roles[ResolutionRole] = "resolution";
  roles[AspectRatioRole] = "resolutionAspectRatio";

  setRoleNames(roles);

  m_resolutions = m_settings->resolutions(m_aspectRatio);
}

VideoResolutionModel::~VideoResolutionModel() {
  m_settings = 0;
}

int VideoResolutionModel::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid()) {
    return m_resolutions.size();
  }

  return 0;
}

QVariant VideoResolutionModel::data(const QModelIndex& index, int role) const {
  if (index.row() < 0 || index.row() >= m_resolutions.size()) {
    return QVariant();
  }

  const QtCamVideoResolution& res = m_resolutions[index.row()];

  switch (role) {
  case IdRole:
    return res.id();

  case NameRole:
    return res.name();

  case CaptureRole:
    return res.captureResolution();

  case PreviewRole:
    return res.previewResolution();

  case FpsRole:
    return res.frameRate();

  case NightFpsRole:
    return res.nightFrameRate();

  case ResolutionRole:
    return res.resolution();

  case AspectRatioRole:
    return res.aspectRatio();

  default:
    return QVariant();
  }
}

QString VideoResolutionModel::aspectRatio() const {
  return m_aspectRatio;
}

void VideoResolutionModel::setAspectRatio(const QString& aspectRatio) {
  if (aspectRatio != m_aspectRatio) {

    m_aspectRatio = aspectRatio;

    beginResetModel();

    m_resolutions = m_settings->resolutions(m_aspectRatio);

    endResetModel();

    emit aspectRatioChanged();
    emit countChanged();
  }
}

int VideoResolutionModel::count() const {
  return rowCount();
}

#if defined(QT5)
QHash<int, QByteArray> VideoResolutionModel::roleNames() const {
  return m_roles;
}

void VideoResolutionModel::setRoleNames(const QHash<int, QByteArray>& roles) {
  m_roles = roles;
}
#endif
