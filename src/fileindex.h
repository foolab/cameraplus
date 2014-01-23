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

#ifndef FILE_INDEX_H
#define FILE_INDEX_H

#include <QString>

class Settings;

class FileIndex {
public:
  FileIndex(Settings *settings);
  virtual ~FileIndex();

  typedef enum {
    Image,
    Video,
  } Type;

  virtual QString stamp(const Type& type) = 0;
  virtual void setStamp(const Type& type, const QString& stamp) = 0;

  virtual int counter(const Type& type) = 0;
  virtual void setCounter(const Type& type, int counter) = 0;

protected:
  Settings *m_settings;
};

class SingleFileIndex : public FileIndex {
public:
  SingleFileIndex(Settings *settings);

  QString stamp(const Type& type);
  void setStamp(const Type& type, const QString& stamp);

  int counter(const Type& type);
  void setCounter(const Type& type, int counter);
};

class MultiFileIndex : public FileIndex {
public:
  MultiFileIndex(Settings *settings);

  QString stamp(const Type& type);
  void setStamp(const Type& type, const QString& stamp);

  int counter(const Type& type);
  void setCounter(const Type& type, int counter);
};

#endif /* FILE_INDEX_H */
