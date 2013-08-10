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
#include <QMultiMap>

class QTemporaryFile;

class MountProtector : public QObject {
  Q_OBJECT

public:
  MountProtector(QObject *parent = 0);
  ~MountProtector();

  Q_INVOKABLE bool lock(const QString& path);

public slots:
  void unlock(const QString& path);
  void unlockAll();

private:
  QMultiMap<QString, QTemporaryFile *> m_locks;
};

#endif /* MOUNT_PROTECTOR_H */
