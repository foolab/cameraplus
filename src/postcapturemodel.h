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

#ifndef POST_CAPTURE_MODEL_H
#define POST_CAPTURE_MODEL_H

#include <QAbstractListModel>
#include <QUrl>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

class QDir;

class PostCaptureModelItem : public QObject {
  Q_OBJECT

  Q_PROPERTY(QUrl url READ url CONSTANT);
  Q_PROPERTY(QString mimeType READ mimeType CONSTANT);
  Q_PROPERTY(QString fileName READ fileName CONSTANT);
  Q_PROPERTY(bool video READ isVideo CONSTANT);

public:
  PostCaptureModelItem(const QString& path, QObject *parent);
  ~PostCaptureModelItem();

  bool init();

  QUrl url() const;
  const QString& path() const;
  QString mimeType() const;
  QString fileName() const;
  bool isVideo() const;
  inline const struct stat& stat() const;

private:
  struct stat m_stat;
  QString m_path;
};

class PostCaptureModel : public QAbstractListModel {
  Q_OBJECT

  Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged);
  Q_PROPERTY(QString videoPath READ videoPath WRITE setVideoPath NOTIFY videoPathChanged);

public:
  PostCaptureModel(QObject *parent = 0);
  ~PostCaptureModel();

  typedef enum {
    MediaRole = Qt::UserRole + 1,
  } Roles;

  QString imagePath() const;
  void setImagePath(const QString& path);

  QString videoPath() const;
  void setVideoPath(const QString& path);

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

public slots:
  void reload();
  void clear();
  void remove(const QUrl& file);

signals:
  void imagePathChanged();
  void videoPathChanged();

private:
  void loadDir(const QDir& dir, QList<PostCaptureModelItem *>& out);

  QString m_imagePath;
  QString m_videoPath;

  QList<PostCaptureModelItem *> m_items;

#if defined(QT5)
  QHash<int, QByteArray> roleNames() const;
  void setRoleNames(const QHash<int, QByteArray>& roles);
  QHash<int, QByteArray> m_roles;
#endif
};

#endif /* POST_CAPTURE_MODEL_H */
