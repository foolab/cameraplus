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
#include <QDateTime>
#include <QFile>
#include "settings.h"
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

FileNaming::FileNaming(QObject *parent) :
  QObject(parent),
  m_settings(0),
  m_index(0) {

}

FileNaming::~FileNaming() {
  delete m_index; m_index = 0;
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
  return fileName(m_imagePath, m_imageSuffix, Image);
}

QString FileNaming::videoFileName() {
  return fileName(m_videoPath, m_videoSuffix, Video);
}

QString FileNaming::fileName(const QString& path, const QString& suffix, const Type& type) {
  FileIndex::Type indexType = (FileIndex::Type) type;

  if (!m_settings) {
    qmlInfo(this) << "settings has not been set";
    return QString();
  }

  if (suffix.isEmpty()) {
    qmlInfo(this) << "called with empty suffix";
    return QString();
  }

  if (path.isEmpty()) {
    qmlInfo(this) << "called with empty path";
    return QString();
  }

  QString date;
  if (m_settings && !m_settings->isUtcForFileNamingUsed()) {
    date = QDate::currentDate().toString("yyyyMMdd");
  }
  else {
    date = QDateTime::currentDateTime().toUTC().date().toString("yyyyMMdd");
  }
  QDir dir(path);

  // index is the last used index
  int index = 0;

  if (m_index->stamp(indexType) != date) {
    m_index->setStamp(indexType, date);
  }
  else {
    index = m_index->counter(indexType);
  }

  if (index == 0) {
    QStringList filters(QString("*%1_*").arg(date));
    QStringList entries = dir.entryList(filters, QDir::Files, QDir::Name);
    if (!entries.isEmpty()) {
      QString name = QFile(entries.last()).fileName();
      index = name.section('_', 1, 1).section('.', 0, 0).toInt();
    }
  }

  while (index < INT_MAX) {
    ++index;

    QString name = QString("%1%2_%3.%4")
      .arg(path).arg(date).arg(QString().sprintf("%03i", index)).arg(suffix);

    if (!QFile(name).exists()) {
      m_index->setCounter(indexType, index);
      return name;
    }

  }

  qmlInfo(this) << "Failed to guess a file name";

  return QString();
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

Settings *FileNaming::settings() const {
  return m_settings;
}

void FileNaming::setSettings(Settings *settings) {
  if (m_settings != settings) {
    m_settings = settings;

    emit settingsChanged();
  }
}

void FileNaming::classBegin() {
  // Nothing
}

void FileNaming::componentComplete() {
  if (QDir(m_imagePath).canonicalPath() == QDir(m_videoPath).canonicalPath()) {
    m_index = new SingleFileIndex(m_settings);
  }
  else {
    m_index = new MultiFileIndex(m_settings);
  }
}
