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

#include "mountprotector.h"
#include <QDir>
#include <QTemporaryFile>
#include <QDeclarativeInfo>

MountProtector::MountProtector(QObject *parent) :
  QObject(parent), m_file(0) {

}

MountProtector::~MountProtector() {
  unlock();
}

QString MountProtector::path() const {
  return m_path;
}

void MountProtector::setPath(const QString& path) {
  if (m_path != path) {
    m_path = path;
    emit pathChanged();
  }
}

bool MountProtector::lock() {
  if (m_file) {
    return true;
  }

  if (m_path.isEmpty()) {
    return false;
  }

  QString path = QString("%1%2.cameraplus_tmp_XXXXXX").arg(m_path).arg(QDir::separator());
  m_file = new QTemporaryFile(path);
  m_file->setAutoRemove(true);
  if (!m_file->open()) {
    qmlInfo(this) << "Failed to lock" << m_file->errorString();
    delete m_file;
    m_file = 0;
    return false;
  }

  if (!QFile::remove(m_file->fileName())) {
    qmlInfo(this) << "Failed to remove temporarily file" << m_file->fileName();
  }

  return true;
}

void MountProtector::unlock() {
  if (m_file) {
    delete m_file;
    m_file = 0;
  }
}
