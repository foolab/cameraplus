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

#ifndef RESOLUTION_MODEL_H
#define RESOLUTION_MODEL_H

#include <QAbstractListModel>

class QtCamImageSettings;
class QtCamVideoSettings;
class QtCamResolution;

class ResolutionModel : public QAbstractListModel {
  Q_OBJECT

  Q_PROPERTY(QString aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY aspectRatioChanged);
  Q_PROPERTY(int count READ count NOTIFY countChanged);

public:

  enum ResolutionRoles {
    IdRole = Qt::UserRole + 1,
    NameRole,
    CaptureRole,
    PreviewRole,
    FpsRole,
    NightFpsRole,
    MegaPixelsRole,
    ResolutionRole,
    AspectRatioRole,
  };

  ResolutionModel(QtCamImageSettings *settings, QObject *parent = 0);
  ResolutionModel(QtCamVideoSettings *settings, QObject *parent = 0);
  ~ResolutionModel();

  int rowCount(const QModelIndex& parent = QModelIndex()) const;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  QString aspectRatio() const;
  void setAspectRatio(const QString& aspectRatio);

  int count() const;

signals:
  void aspectRatioChanged();
  void countChanged();

private:
  void init();

  QString m_aspectRatio;

  QtCamImageSettings *m_image;
  QtCamVideoSettings *m_video;

  QList<QtCamResolution> m_resolutions;

#if defined(QT5)
  QHash<int, QByteArray> roleNames() const;
  void setRoleNames(const QHash<int, QByteArray>& roles);
  QHash<int, QByteArray> m_roles;
#endif
};

#endif /* RESOLUTION_MODEL_H */
