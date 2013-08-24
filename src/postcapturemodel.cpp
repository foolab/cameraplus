/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
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

#include "postcapturemodel.h"
#include "fileinfomodel.h"
#include <QDir>
#include <QDateTime>
#include <QUrl>

static QHash<QString, QString> m_mime;

PostCaptureModel::PostCaptureModel(QObject *parent) :
  QSortFilterProxyModel(parent),
  m_model(new FileInfoModel(this)) {

  QHash<int, QByteArray> roles;
  roles.insert(UrlRole, "url");
  roles.insert(TitleRole, "title");
  roles.insert(MimeTypeRole, "mimeType");
  roles.insert(CreatedRole, "created");
  roles.insert(FileNameRole, "fileName");

  setSourceModel(m_model);

  setDynamicSortFilter(true);
  setSortRole(CreatedRole);

  setRoleNames(roles);

  if (m_mime.isEmpty()) {
    m_mime.insert("jpg", "image/jpeg");
    m_mime.insert("mp4", "video/mp4");
  }
}

PostCaptureModel::~PostCaptureModel() {

}

QString PostCaptureModel::imagePath() const {
  return m_imagePath;
}

void PostCaptureModel::setImagePath(const QString& path) {
  if (m_imagePath != path) {
    m_imagePath = path;
    emit imagePathChanged();
  }
}

QString PostCaptureModel::videoPath() const {
  return m_videoPath;
}

void PostCaptureModel::setVideoPath(const QString& path) {
  if (m_videoPath != path) {
    m_videoPath = path;
    emit videoPathChanged();
  }
}

QStringList PostCaptureModel::loadDir(QDir& dir) {
  QStringList files;
  QStringList entries(dir.entryList(QDir::Files | QDir::NoDotAndDotDot));

  foreach (const QString& entry, entries) {
    files << dir.absoluteFilePath(entry);
  }

  return files;
}

void PostCaptureModel::reload() {
  QStringList files;

  QDir images(m_imagePath);
  QDir videos(m_videoPath);

  if (images.canonicalPath() == videos.canonicalPath()) {
    files += loadDir(images);
  }
  else {
    files += loadDir(images);
    files += loadDir(videos);
  }

  m_data.clear();

  m_model->setFiles(files);

  sort(0, Qt::DescendingOrder);
}

bool PostCaptureModel::lessThan(const QModelIndex& left, const QModelIndex& right) const {
  return info(sourceModel()->data(left, Qt::DisplayRole).toString()).created() <
    info(sourceModel()->data(right, Qt::DisplayRole).toString()).created();
}

QVariant PostCaptureModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid() || index.row() < 0) {
    return QVariant();
  }

  QFileInfo inf = info(QSortFilterProxyModel::data(index, Qt::DisplayRole).toString());
  switch (role) {
  case UrlRole:
    return QUrl::fromLocalFile(inf.absoluteFilePath());

  case TitleRole:
    return inf.baseName();

  case MimeTypeRole:
    if (m_mime.contains(inf.completeSuffix())) {
      return m_mime[inf.completeSuffix()];
    }

    return QString();

  case CreatedRole:
    return inf.created().toString();

  case FileNameRole:
    return inf.fileName();

  default:
    break;
  }

  return QVariant();
}

QFileInfo PostCaptureModel::info(const QString& path) const {
  if (m_data.contains(path)) {
    return m_data[path];
  }

  QFileInfo inf(path);
  m_data.insert(path, inf);

  return inf;
}

void PostCaptureModel::remove(const QUrl& file) {
  QString path(file.toLocalFile());

  m_data.remove(path);
  m_model->removeFile(path);
}

#if defined(QT5)
QHash<int, QByteArray> PostCaptureModel::roleNames() const {
  return m_roles;
}

void PostCaptureModel::setRoleNames(const QHash<int, QByteArray>& roles) {
  m_roles = roles;
}
#endif
