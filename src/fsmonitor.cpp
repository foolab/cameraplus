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

#include "fsmonitor.h"
#include <qmusbmode.h>
#include <sys/statvfs.h>
#include <cstdio>

#define MIN_SPACE 100 * 1024 * 1024 // 100 MB

FSMonitor::FSMonitor(QObject *parent) :
  QObject(parent), m_mode(new MeeGo::QmUSBMode(this)) {

  QObject::connect(m_mode, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)),
		   this, SLOT(modeChanged()));

  m_available =
    m_mode->mountStatus(MeeGo::QmUSBMode::DocumentDirectoryMount)
    .testFlag(MeeGo::QmUSBMode::ReadWriteMount);
}

FSMonitor::~FSMonitor() {

}

bool FSMonitor::isAvailable() const {
  return m_available;
}

void FSMonitor::setAvailable(bool available) {
  if (m_available != available) {
    m_available = available;
    emit availabilityChanged();
  }
}

void FSMonitor::modeChanged() {
  setAvailable(m_mode->mountStatus(MeeGo::QmUSBMode::DocumentDirectoryMount)
	       .testFlag(MeeGo::QmUSBMode::ReadWriteMount));
}

bool FSMonitor::hasFreeSpace(const QString& path) {
  struct statvfs buf;

  if (statvfs(path.toLocal8Bit().data(), &buf) == -1) {
    std::perror("statvfs");
    return false;
  }

  return (buf.f_bsize * buf.f_bavail >= MIN_SPACE);
}
