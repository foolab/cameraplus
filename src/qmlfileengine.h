// -*- c++ -*-

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

#ifndef QML_FILE_ENGINE_H
#define QML_FILE_ENGINE_H

#include <QFSFileEngine>

class QResource;

class QmlFileEngine : public QFSFileEngine {
public:
  QmlFileEngine(const QString& fileName);

  virtual void setFileName(const QString& file);
  virtual bool open(QIODevice::OpenMode flags);
  virtual bool close();
  virtual bool flush();
  virtual qint64 size() const;
  virtual qint64 pos() const;
  virtual bool atEnd() const;
  virtual bool seek(qint64 pos);
  virtual qint64 read(char *data, qint64 len);
  virtual qint64 write(const char *data, qint64 len);
  virtual bool remove();
  virtual bool copy(const QString& newName);
  virtual bool rename(const QString& newName);
  virtual bool link(const QString& newName);
  virtual bool isSequential() const;
  virtual bool isRelativePath() const;
  virtual bool mkdir(const QString& dirName, bool createParentDirectories) const;
  virtual bool rmdir(const QString& dirName, bool recurseParentDirectories) const;
  virtual bool setSize(qint64 size);
  virtual QStringList entryList(QDir::Filters filters, const QStringList& filterNames) const;
  virtual bool caseSensitive() const;
  virtual FileFlags fileFlags(FileFlags type) const;
  virtual bool setPermissions(uint perms);
  virtual QString fileName(QAbstractFileEngine::FileName file) const;
  virtual uint ownerId(FileOwner owner) const;
  virtual QString owner(FileOwner owner) const;
  virtual QDateTime fileTime(FileTime time) const;

private:
  QResource *m_res;
  QByteArray m_data;
  qint64 m_off;
};

#endif /* QML_FILE_ENGINE_H */
