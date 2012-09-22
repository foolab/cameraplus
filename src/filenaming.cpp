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

#include "filenaming.h"
#include <QDir>
#include <QDebug>
#include <QDate>
#include <QFile>

#define PATH QString("%1%2MyDocs%2cameraplus%2").arg(QDir::homePath()).arg(QDir::separator())

FileNaming::FileNaming(QObject *parent) :
  QObject(parent) {

}

FileNaming::~FileNaming() {

}

QString FileNaming::imageSuffix() const {
  return m_image;
}

void FileNaming::setImageSuffix(const QString& suffix) {
  if (m_image != suffix) {
    m_image = suffix;
    emit imageSuffixChanged();
  }
}

QString FileNaming::videoSuffix() const {
  return m_video;
}

void FileNaming::setVideoSuffix(const QString& suffix) {
  if (m_video != suffix) {
    m_video = suffix;
    emit videoSuffixChanged();
  }
}

QString FileNaming::imageFileName() {
  return fileName(m_image);
}

QString FileNaming::videoFileName() {
  return fileName(m_video);
}

QString FileNaming::fileName(const QString& suffix) {
  if (suffix.isEmpty()) {
    return QString();
  }

  if (!QDir::root().mkpath(PATH)) {
    qWarning() << "Failed to create" << PATH;
    return QString();
  }

  // Shamelessly stolen from Aura
  QDir dir(PATH);
  QString date = QDate::currentDate().toString("yyyyMMdd");

  QStringList filters(QString("*%1_*").arg(date));
  QStringList entries = dir.entryList(filters, QDir::Files, QDir::Name);

  int index = 0;

  if (!entries.isEmpty()) {
    QString name = QFile(entries.last()).fileName();
    index = name.section('_', 1, 1).section('.', 0, 0).toInt();
  }

  ++index;

  QString name = QString("%1%2_%3.%4").arg(PATH).arg(date).arg(QString().sprintf("%03i", index)).
    arg(suffix);

  return name;
}
