// -*- c++ -*-

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

#ifndef POST_CAPTURE_MODEL_H
#define POST_CAPTURE_MODEL_H

#include <QStandardItemModel>

class QDir;

class PostCaptureModel : public QStandardItemModel {
  Q_OBJECT

  Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged);
  Q_PROPERTY(QString videoPath READ videoPath WRITE setVideoPath NOTIFY videoPathChanged);

public:
  PostCaptureModel(QObject *parent = 0);
  ~PostCaptureModel();

  typedef enum {
    UrlRole = Qt::UserRole + 1,
    MimeTypeRole = Qt::UserRole + 2,
    FileNameRole = Qt::UserRole + 3,
  } Roles;

  QString imagePath() const;
  void setImagePath(const QString& path);

  QString videoPath() const;
  void setVideoPath(const QString& path);

public slots:
  void reload();
  void clear();
  void remove(const QUrl& file);

signals:
  void imagePathChanged();
  void videoPathChanged();

private:
  QString m_imagePath;
  QString m_videoPath;

  void loadDir(const QDir& dir, QList<QStandardItem *>& out);

#if defined(QT5)
  QHash<int, QByteArray> roleNames() const;
  void setRoleNames(const QHash<int, QByteArray>& roles);
  QHash<int, QByteArray> m_roles;
#endif
};

#endif /* POST_CAPTURE_MODEL_H */
