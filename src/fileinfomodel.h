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

#ifndef FILE_INFO_MODEL_H
#define FILE_INFO_MODEL_H

#include <QAbstractListModel>
#include <QStringList>

class FileInfoModel : public QAbstractListModel {
  Q_OBJECT

public:
  FileInfoModel(QObject *parent = 0);
  ~FileInfoModel();

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  void setFiles(const QStringList& files);

  void appendFile(const QString& file);
  void appendFiles(const QStringList& files);

  void removeFile(const QString& file);

private:
  void clear();

  QStringList m_info;
};

#endif /* FILE_INFO_MODEL_H */
