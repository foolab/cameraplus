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

#include "filenaming.h"
#include <QDir>
#include <QDate>
#include <QDateTime>
#include <QFile>
#include "platformsettings.h"
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

FileNaming::FileNaming(QObject *parent) :
  QObject(parent),
  m_settings(0),
  m_platformSettings(0),
  m_index(0),
  m_fileNamingInUtc(true) {

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

QString FileNaming::imageFileNameWithExtension(const QString& ext) {
  return fileName(m_platformSettings->imagePath(), ext, Image);
}

QString FileNaming::imageFileName() {
  return fileName(m_platformSettings->imagePath(), m_imageSuffix, Image);
}

QString FileNaming::videoFileName() {
  return fileName(m_platformSettings->videoPath(), m_videoSuffix, Video);
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

  QString date = m_fileNamingInUtc ?
    QDateTime::currentDateTime().toUTC().date().toString("yyyyMMdd") :
    QDateTime::currentDateTime().date().toString("yyyyMMdd");
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

QString FileNaming::temporaryVideoFileName() {
  QString temp = m_platformSettings->temporaryVideoPath();

  if (temp.isEmpty()) {
    return QString();
  }

  return QString("%1.cameraplus_video.tmp").arg(temp);
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

PlatformSettings *FileNaming::platformSettings() const {
  return m_platformSettings;
}

void FileNaming::setPlatformSettings(PlatformSettings *settings) {
  if (m_platformSettings != settings) {
    m_platformSettings = settings;
    emit platformSettingsChanged();
  }
}

void FileNaming::classBegin() {
  // Nothing
}

void FileNaming::componentComplete() {
  if (QDir(m_platformSettings->imagePath()).canonicalPath()
      == QDir(m_platformSettings->videoPath()).canonicalPath()) {
    m_index = new SingleFileIndex(m_settings);
  }
  else {
    m_index = new MultiFileIndex(m_settings);
  }
}

bool FileNaming::isFileNamingInUtc() const {
  return m_fileNamingInUtc;
}

void FileNaming::setFileNamingInUtc(bool inUtc) {
  if (m_fileNamingInUtc != inUtc) {
    m_fileNamingInUtc = inUtc;
    emit fileNamingInUtcChanged();
  }
}
