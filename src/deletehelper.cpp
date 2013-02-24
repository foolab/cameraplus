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

#include "deletehelper.h"
#include <QUrl>
#include <QFile>
#include <QDeclarativeInfo>

DeleteHelper::DeleteHelper(QObject *parent) :
  QObject(parent) {

}

DeleteHelper::~DeleteHelper() {

}

bool  DeleteHelper::remove(const QUrl& path) {
  QFile file(path.toLocalFile());

  if (!file.exists()) {
    return true;
  }

  if (!file.remove()) {
    qmlInfo(this) << "Failed to remove file" << file.fileName() << file.errorString();
    return false;
  }

  return true;
}
