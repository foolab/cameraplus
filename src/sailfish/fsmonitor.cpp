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

#include "fsmonitor.h"
#include <sys/statvfs.h>
#include <cstdio>

#define MIN_SPACE 100 * 1024 * 1024 // 100 MB

FSMonitor::FSMonitor(QObject *parent) :
  QObject(parent) {

}

FSMonitor::~FSMonitor() {

}

bool FSMonitor::isAvailable() const {
  return true;
}

bool FSMonitor::hasFreeSpace(const QString& path) {
  struct statvfs buf;

  if (statvfs(path.toLocal8Bit().data(), &buf) == -1) {
    std::perror("statvfs");
    return false;
  }

  // If the user has a lot of free space (8 GB for example)
  // then we will overflow the unsigned long int thus we use a 64 bits "int"
  quint64 freeSpace = (quint64)buf.f_bsize * (quint64)buf.f_bavail;
  return (freeSpace >= MIN_SPACE);
}
