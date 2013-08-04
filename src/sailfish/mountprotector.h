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

#ifndef MOUNT_PROTECTOR_H
#define MOUNT_PROTECTOR_H

#include <QObject>
class QTemporaryFile;

class MountProtector : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged);

public:
  MountProtector(QObject *parent = 0);
  ~MountProtector();

  QString path() const;
  void setPath(const QString& path);

  Q_INVOKABLE bool lock();

public slots:
  void unlock();

signals:
  void pathChanged();

private:
  QString m_path;
  QTemporaryFile *m_file;
};

#endif /* MOUNT_PROTECTOR_H */
