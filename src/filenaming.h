// -*- c++ -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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
  Q_PROPERTY(QString path READ path CONSTANT);

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

  QString path();
  QString temporaryPath();

signals:
  void imageSuffixChanged();
  void videoSuffixChanged();

private:
  QString fileName(const QString& suffix);
  QString canonicalPath(const QString& path);

  QString m_image;
  QString m_video;
  QString m_path;
  QString m_temp;
};

#endif /* FILE_NAMING_H */
