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

#include "mountprotector.h"
#include <QDir>
#include <QTemporaryFile>
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif

MountProtector::MountProtector(QObject *parent) :
  QObject(parent) {

}

MountProtector::~MountProtector() {
  unlockAll();
}

bool MountProtector::lock(const QString& path) {
  if (path.isEmpty()) {
    qmlInfo(this) << "Cannot lock an empty path";
    return false;
  }

  QString p = QString("%1%2.cameraplus_tmp_XXXXXX").arg(path).arg(QDir::separator());
  QTemporaryFile *file = new QTemporaryFile(p);
  file->setAutoRemove(true);

  if (!file->open()) {
    qmlInfo(this) << "Failed to lock" << file->errorString();
    delete file;
    file = 0;
    return false;
  }

  if (!QFile::remove(file->fileName())) {
    qmlInfo(this) << "Failed to remove temporarily file" << file->fileName();
  }

  m_locks.insert(path, file);

  return true;
}

void MountProtector::unlock(const QString& path) {
  QTemporaryFile *file = m_locks.take(path);

  if (file) {
    delete file;
  }
}

void MountProtector::unlockAll() {
  qDeleteAll(m_locks);

  m_locks.clear();
}
