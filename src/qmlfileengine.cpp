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

#include "qmlfileengine.h"
#include <QResource>
#include <QDateTime>

// TODO: support mmap extension

QmlFileEngine::QmlFileEngine(const QString& fileName) :
  QFSFileEngine(fileName),
  m_res(new QResource(fileName)),
  m_off(0) {

}

void QmlFileEngine::setFileName(const QString& file) {
  m_res->setFileName(file);
}

bool QmlFileEngine::open(QIODevice::OpenMode flags) {
  if (!m_res->isValid() || m_res->fileName().isEmpty()) {
    return false;
  }

  if (flags & QIODevice::WriteOnly) {
    return false;
  }

  m_off = 0;

  if (m_res->isCompressed()) {
    m_data = qUncompress(m_res->data(), m_res->size());
  }
  else {
    m_data = QByteArray::fromRawData((const char *)m_res->data(), m_res->size());
  }

  m_data.replace("@IMPORT_QT_QUICK@", "import QtQuick 1.1");

  return true;
}

bool QmlFileEngine::close() {
  m_data.clear();
  m_off = 0;

  return true;
}

bool QmlFileEngine::flush() {
  return true;
}

qint64 QmlFileEngine::size() const {
  return m_data.size();
}

qint64 QmlFileEngine::pos() const {
  return m_off;
}

bool QmlFileEngine::atEnd() const {
  return m_off == m_data.size();
}

bool QmlFileEngine::seek(qint64 pos) {
  if (pos > m_data.size()) {
    return false;
  }

  return true;
}

qint64 QmlFileEngine::read(char *data, qint64 len) {
  if (len > m_data.size() - m_off) {
    len = m_data.size() - m_off;
  }

  if (len <= 0) {
    return 0;
  }

  memcpy(data, m_data.data() + m_off, len);

  m_off += len;

  return len;
}

qint64 QmlFileEngine::write(const char *data, qint64 len) {
  Q_UNUSED(data);
  Q_UNUSED(len);

  return false;
}

bool QmlFileEngine::remove() {
  return false;
}

bool QmlFileEngine::copy(const QString& newName) {
  Q_UNUSED(newName);

  return false;
}

bool QmlFileEngine::rename(const QString& newName) {
  Q_UNUSED(newName);

  return false;
}

bool QmlFileEngine::link(const QString& newName) {
  Q_UNUSED(newName);

  return false;
}

bool QmlFileEngine::isSequential() const {
  return false;
}

bool QmlFileEngine::isRelativePath() const {
  return false;
}

bool QmlFileEngine::mkdir(const QString& dirName, bool createParentDirectories) const {
  Q_UNUSED(dirName);
  Q_UNUSED(createParentDirectories);

  return false;
}

bool QmlFileEngine::rmdir(const QString& dirName, bool recurseParentDirectories) const {
  Q_UNUSED(dirName);
  Q_UNUSED(recurseParentDirectories);

  return false;
}

bool QmlFileEngine::setSize(qint64 size) {
  Q_UNUSED(size);

  return false;
}

QStringList QmlFileEngine::entryList(QDir::Filters filters, const QStringList& filterNames) const {
  Q_UNUSED(filters);
  Q_UNUSED(filterNames);

  return QStringList();
}

bool QmlFileEngine::caseSensitive() const {
  return true;
}

QFSFileEngine::FileFlags QmlFileEngine::fileFlags(FileFlags type) const {
  QFSFileEngine::FileFlags flags = 0;

  if (!m_res->isValid()) {
    return flags;
  }

  if (type & TypesMask) {
    flags |= FileType;
  }

  if (type & FlagsMask) {
    flags |= ExistsFlag;
  }

  return flags;
}

bool QmlFileEngine::setPermissions(uint perms) {
  Q_UNUSED(perms);

  return false;
}

QString QmlFileEngine::fileName(QAbstractFileEngine::FileName file) const {
  switch (file) {
  case QAbstractFileEngine::AbsoluteName:
    return m_res->fileName();
  default:
    break;
  }

  // TODO:
  return QString();
}

uint QmlFileEngine::ownerId(FileOwner owner) const {
  Q_UNUSED(owner);

  return -2;
}

QString QmlFileEngine::owner(FileOwner owner) const {
  Q_UNUSED(owner);

  return QString();
}

QDateTime QmlFileEngine::fileTime(FileTime time) const {
  Q_UNUSED(time);

  return QDateTime();
}
