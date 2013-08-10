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

#ifndef FILE_NAMING_H
#define FILE_NAMING_H

#include <QObject>

class FileNaming : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString imageSuffix READ imageSuffix WRITE setImageSuffix NOTIFY imageSuffixChanged);
  Q_PROPERTY(QString videoSuffix READ videoSuffix WRITE setVideoSuffix NOTIFY videoSuffixChanged);
  Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged);
  Q_PROPERTY(QString videoPath READ videoPath WRITE setVideoPath NOTIFY videoPathChanged);
  Q_PROPERTY(QString temporaryVideoPath READ temporaryVideoPath WRITE setTemporaryVideoPath NOTIFY temporaryVideoPathChanged);

public:
  FileNaming(QObject *parent = 0);
  ~FileNaming();

  QString imageSuffix() const;
  void setImageSuffix(const QString& suffix);

  QString videoSuffix() const;
  void setVideoSuffix(const QString& suffix);

  Q_INVOKABLE QString imageFileName();
  Q_INVOKABLE QString videoFileName();
  Q_INVOKABLE QString temporaryVideoFileName();

  QString imagePath() const;
  void setImagePath(const QString& path);

  QString videoPath() const;
  void setVideoPath(const QString& path);

  QString temporaryVideoPath() const;
  void setTemporaryVideoPath(const QString& path);

signals:
  void imageSuffixChanged();
  void videoSuffixChanged();
  void imagePathChanged();
  void videoPathChanged();
  void temporaryVideoPathChanged();

private:
  QString fileName(const QString& path, const QString& suffix);
  QString canonicalPath(const QString& path);
  QString temporaryPath();

  QString m_imageSuffix;
  QString m_videoSuffix;
  QString m_imagePath;
  QString m_videoPath;
  QString m_temporaryVideoPath;

  QString m_date;
  int m_index;
};

#endif /* FILE_NAMING_H */
