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

#include "fileindex.h"
#include "settings.h"

FileIndex::FileIndex(Settings *settings) :
  m_settings(settings) {

}

FileIndex::~FileIndex() {

}

SingleFileIndex::SingleFileIndex(Settings *settings) :
  FileIndex(settings) {

}

QString SingleFileIndex::stamp(const Type& type) {
  Q_UNUSED(type);

  return m_settings->fileNamingStamp("stamp");
}

void SingleFileIndex::setStamp(const Type& type, const QString& stamp) {
  Q_UNUSED(type);

  m_settings->setFileNamingStamp("stamp", stamp);
}

int SingleFileIndex::counter(const Type& type) {
  Q_UNUSED(type);

  return m_settings->fileNamingCounter("counter");
}

void SingleFileIndex::setCounter(const Type& type, int counter) {
  Q_UNUSED(type);

  m_settings->setFileNamingCounter("counter", counter);
}

MultiFileIndex::MultiFileIndex(Settings *settings) :
  FileIndex(settings) {

}

QString MultiFileIndex::stamp(const Type& type) {
  switch (type) {
  case Image:
    return m_settings->fileNamingStamp("imageStamp");
  case Video:
    return m_settings->fileNamingStamp("videoStamp");
  }
}

void MultiFileIndex::setStamp(const Type& type, const QString& stamp) {
  switch (type) {
  case Image:
    m_settings->setFileNamingStamp("imageStamp", stamp);
    break;
  case Video:
    m_settings->setFileNamingStamp("videoStamp", stamp);
    break;
  }
}

int MultiFileIndex::counter(const Type& type) {
  switch (type) {
  case Image:
    return m_settings->fileNamingCounter("imageCounter");
  case Video:
    return m_settings->fileNamingCounter("videoCounter");
  }
}

void MultiFileIndex::setCounter(const Type& type, int counter) {
  switch (type) {
  case Image:
    m_settings->setFileNamingCounter("imageCounter", counter);
    break;
  case Video:
    m_settings->setFileNamingCounter("videoCounter", counter);
    break;
  }
}
