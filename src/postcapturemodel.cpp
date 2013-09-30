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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

static QHash<QString, QString> m_mime;

class PostCaptureModelItem : public QStandardItem {
public:
  PostCaptureModelItem(const QString& path) :
    m_path(path) {

  }

  ~PostCaptureModelItem() {

  }

  bool init() {
    if (::stat(m_path.toLocal8Bit().constData(), &m_stat) != 0) {
      perror("stat");
      return false;
    }

    return true;
  }

  int type() const {
    return UserType;
  }

  inline const QString& path() const {
    return m_path;
  }

  inline QString mimeType(const QString& ext) const {
    if (m_mime.contains(ext)) {
      return m_mime[ext];
    }

    return QString();
  }

  QVariant data (int role = Qt::UserRole + 1) const {
    switch (role) {
    case PostCaptureModel::UrlRole:
      return QUrl::fromLocalFile(m_path);

    case PostCaptureModel::MimeTypeRole:
      return mimeType(m_path.mid(m_path.lastIndexOf('.') + 1));

    case PostCaptureModel::FileNameRole:
      return m_path.mid(m_path.lastIndexOf('/') + 1);

    default:
      return QVariant();
    }
  }

  inline const struct stat& stat() const {
    return m_stat;
  }

private:
  struct stat m_stat;

  QString m_path;
};

static bool lessThan(const QStandardItem *a1, const QStandardItem *a2) {
  // we sort descending
  return dynamic_cast<const PostCaptureModelItem *>(a1)->stat().st_mtime >
    dynamic_cast<const PostCaptureModelItem *>(a2)->stat().st_mtime;
}

PostCaptureModel::PostCaptureModel(QObject *parent) :
  QStandardItemModel(parent) {

  QHash<int, QByteArray> roles;
  roles.insert(UrlRole, "url");
  roles.insert(MimeTypeRole, "mimeType");
  roles.insert(FileNameRole, "fileName");

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

void PostCaptureModel::loadDir(const QDir& dir, QList<QStandardItem *>& out) {
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

    PostCaptureModelItem *item = new PostCaptureModelItem(dir.absoluteFilePath(entry->d_name));
    if (!item->init()) {
      delete item;
      continue;
    }

    out << item;
  }

  closedir(d);
}

void PostCaptureModel::reload() {
  QList<QStandardItem *> files;

  QDir images(m_imagePath);
  QDir videos(m_videoPath);

  loadDir(images, files);

  if (images.canonicalPath() != videos.canonicalPath()) {
    loadDir(videos, files);
  }

  qSort(files.begin(), files.end(), lessThan);

  invisibleRootItem()->appendRows(files);
}

void PostCaptureModel::clear() {
  QStandardItemModel::clear();
}

void PostCaptureModel::remove(const QUrl& file) {
  QString path(file.toLocalFile());

  int count = invisibleRootItem()->rowCount();

  for (int x = 0; x < count; x++) {
    if (dynamic_cast<PostCaptureModelItem *>(invisibleRootItem()->child(x))->path() == path) {
      invisibleRootItem()->removeRow(x);
      return;
    }
  }
}

#if defined(QT5)
QHash<int, QByteArray> PostCaptureModel::roleNames() const {
  return m_roles;
}

void PostCaptureModel::setRoleNames(const QHash<int, QByteArray>& roles) {
  m_roles = roles;
}
#endif
