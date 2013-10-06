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
#include <QDir>
#include <QDateTime>
#include <QUrl>
#include <QStandardItem>
#include <dirent.h>
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

static QHash<QString, QString> m_mime;


PostCaptureModelItem::PostCaptureModelItem(const QString& path, QObject *parent) :
  QObject(parent),
  m_path(path) {

}

PostCaptureModelItem::~PostCaptureModelItem() {

}

bool PostCaptureModelItem::init() {
  if (::stat(m_path.toLocal8Bit().constData(), &m_stat) != 0) {
    perror("stat");
    return false;
  }

  return true;
}

QUrl PostCaptureModelItem::url() const {
  return QUrl::fromLocalFile(m_path);
}

const QString& PostCaptureModelItem::path() const {
  return m_path;
}

QString PostCaptureModelItem::mimeType() const {
  QString ext = m_path.mid(m_path.lastIndexOf('.') + 1);

  if (m_mime.contains(ext)) {
    return m_mime[ext];
  }

  return QString();
}

QString PostCaptureModelItem::fileName() const {
  return m_path.mid(m_path.lastIndexOf('/') + 1);
}

bool PostCaptureModelItem::isVideo() const {
  return mimeType().startsWith("video/");
}

inline const struct stat& PostCaptureModelItem::stat() const {
  return m_stat;
}

static bool lessThan(const PostCaptureModelItem *a1, const PostCaptureModelItem *a2) {
  // we sort descending
  return dynamic_cast<const PostCaptureModelItem *>(a1)->stat().st_mtime >
    dynamic_cast<const PostCaptureModelItem *>(a2)->stat().st_mtime;
}

PostCaptureModel::PostCaptureModel(QObject *parent) :
  QAbstractListModel(parent) {

  QHash<int, QByteArray> roles;
  roles.insert(MediaRole, "media");

  setRoleNames(roles);

  if (m_mime.isEmpty()) {
    m_mime.insert("jpg", "image/jpeg");
    m_mime.insert("png", "image/png");
    m_mime.insert("mp4", "video/mp4");
    m_mime.insert("avi", "video/x-msvideo");
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

void PostCaptureModel::loadDir(const QDir& dir, QList<PostCaptureModelItem *>& out) {
  QString path = dir.canonicalPath();

  DIR *d = opendir(path.toLocal8Bit().constData());
  if (!d) {
    qmlInfo(this) << "Failed to open dir" << path;
    return;
  }

  while (true) {
    struct dirent *entry = readdir(d);
    if (!entry) {
      break;
    }

    if (QLatin1String(".") == entry->d_name || QLatin1String("..") == entry->d_name) {
      continue;
    }

    if (!(entry->d_type == DT_LNK || entry->d_type == DT_REG)) {
      continue;
    }

    PostCaptureModelItem *item =
      new PostCaptureModelItem(dir.absoluteFilePath(entry->d_name), this);
    if (!item->init()) {
      delete item;
      continue;
    }

    out << item;
  }

  closedir(d);
}

void PostCaptureModel::reload() {
  clear();

  QList<PostCaptureModelItem *> files;

  QDir images(m_imagePath);
  QDir videos(m_videoPath);

  loadDir(images, files);

  if (images.canonicalPath() != videos.canonicalPath()) {
    loadDir(videos, files);
  }

  qSort(files.begin(), files.end(), lessThan);

  if (files.isEmpty()) {
    return;
  }

  beginInsertRows(QModelIndex(), 0, files.size() - 1);
  m_items = files;
  endInsertRows();
}

void PostCaptureModel::clear() {
  if (m_items.isEmpty()) {
    return;
  }

  beginRemoveRows(QModelIndex(), 0, m_items.size() - 1);
  qDeleteAll(m_items);
  endRemoveRows();
}

void PostCaptureModel::remove(const QUrl& file) {
  QString path = file.toLocalFile();

  for (int x = 0; x < m_items.size(); x++) {
    if (m_items[x]->path() == path) {
      beginRemoveRows(QModelIndex(), x, x);
      m_items.takeAt(x)->deleteLater();
      endRemoveRows();
      return;
    }
  }
}

int PostCaptureModel::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid()) {
    return m_items.size();
  }

  return 0;
}

QVariant PostCaptureModel::data(const QModelIndex& index, int role) const {
  if (role == MediaRole && index.row() < m_items.size()) {
    QObject *o = m_items[index.row()];
    return QVariant::fromValue<QObject *>(o);
  }

  return QVariant();
}

#if defined(QT5)
QHash<int, QByteArray> PostCaptureModel::roleNames() const {
  return m_roles;
}

void PostCaptureModel::setRoleNames(const QHash<int, QByteArray>& roles) {
  m_roles = roles;
}
#endif
