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

#include "filenaming.h"
#include <QDir>
#include <QDate>
#include <QFile>
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

#define PATH QString("%1%2MyDocs%2DCIM%2").arg(QDir::homePath()).arg(QDir::separator())
#define TEMP_PATH QString("%1%2MyDocs%2.cameraplus%2").arg(QDir::homePath()).arg(QDir::separator())

FileNaming::FileNaming(QObject *parent) :
  QObject(parent), m_index(-1) {

}

FileNaming::~FileNaming() {

}

QString FileNaming::imageSuffix() const {
  return m_imageSuffix;
}

void FileNaming::setImageSuffix(const QString& suffix) {
  if (m_imageSuffix != suffix) {
    m_imageSuffix = suffix;
    emit imageSuffixChanged();
  }
}

QString FileNaming::videoSuffix() const {
  return m_videoSuffix;
}

void FileNaming::setVideoSuffix(const QString& suffix) {
  if (m_videoSuffix != suffix) {
    m_videoSuffix = suffix;
    emit videoSuffixChanged();
  }
}

QString FileNaming::imageFileName() {
  return fileName(m_imagePath, m_imageSuffix);
}

QString FileNaming::videoFileName() {
  return fileName(m_videoPath, m_videoSuffix);
}

QString FileNaming::fileName(const QString& path, const QString& suffix) {
  QString date = QDate::currentDate().toString("yyyyMMdd");
  QDir dir(path);

  if (suffix.isEmpty()) {
    return QString();
  }

  if (path.isEmpty()) {
    return QString();
  }

  if (date != m_date) {
    m_index = -1;
    m_date = date;
  }

  if (m_index == -1) {
    QStringList filters(QString("*%1_*").arg(date));
    QStringList entries = dir.entryList(filters, QDir::Files, QDir::Name);
    if (entries.isEmpty()) {
      m_index = 0;
    }
    else {
      QString name = QFile(entries.last()).fileName();
      m_index = name.section('_', 1, 1).section('.', 0, 0).toInt();
    }
  }

  ++m_index;

  QString name = QString("%1%2_%3.%4").arg(path).arg(date).arg(QString().sprintf("%03i", m_index)).
    arg(suffix);

  if (QFile(name).exists()) {
    return QString();
  }

  return name;
}

QString FileNaming::canonicalPath(const QString& path) {
  if (!QDir::root().mkpath(path)) {
    qmlInfo(this) << "Failed to create path" << path;
    return QString();
  }

  QString newPath = QFileInfo(path).canonicalFilePath();

  if (newPath.isEmpty()) {
    return newPath;
  }

  if (!newPath.endsWith(QDir::separator())) {
    newPath.append(QDir::separator());
  }

  return newPath;
}

QString FileNaming::temporaryVideoFileName() {
  if (m_temporaryVideoPath.isEmpty()) {
    return QString();
  }

  return QString("%1.cameraplus_video.tmp").arg(m_temporaryVideoPath);
}

QString FileNaming::imagePath() const {
  return m_imagePath;
}

void FileNaming::setImagePath(const QString& path) {
  QString p = canonicalPath(path);

  if (m_imagePath != p) {
    m_imagePath = p;
    emit imagePathChanged();
  }
}

QString FileNaming::videoPath() const {
  return m_videoPath;
}

void FileNaming::setVideoPath(const QString& path) {
  QString p = canonicalPath(path);

  if (m_videoPath != p) {
    m_videoPath = p;
    emit videoPathChanged();
  }
}

QString FileNaming::temporaryVideoPath() const {
  return m_temporaryVideoPath;
}

void FileNaming::setTemporaryVideoPath(const QString& path) {
  QString p = canonicalPath(path);

  if (m_temporaryVideoPath != p) {
    m_temporaryVideoPath = p;
    emit temporaryVideoPathChanged();
  }
}
