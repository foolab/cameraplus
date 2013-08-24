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

#include "fileinfomodel.h"

FileInfoModel::FileInfoModel(QObject *parent) :
  QAbstractListModel(parent) {

}

FileInfoModel::~FileInfoModel() {

}

int FileInfoModel::rowCount(const QModelIndex& parent) const {
  if (parent.isValid()) {
    return 0;
  }

  return m_info.size();
}

QVariant FileInfoModel::data(const QModelIndex& index, int role) const {
  if (index.isValid() && index.row() < m_info.count() && role == Qt::DisplayRole) {
    return m_info[index.row()];
  }

  return QVariant();
}

void FileInfoModel::setFiles(const QStringList& files) {
  clear();

  appendFiles(files);
}

void FileInfoModel::appendFile(const QString& file) {
  appendFiles(QStringList() << file);
}

void FileInfoModel::appendFiles(const QStringList& files) {
  if (files.isEmpty()) {
    return;
  }

  beginInsertRows(QModelIndex(), m_info.size(), files.size() - 1);
  m_info << files;
  endInsertRows();
}

void FileInfoModel::removeFile(const QString& file) {
  int index = m_info.indexOf(file);

  if (index == -1) {
    return;
  }

  beginRemoveRows(QModelIndex(), index, index);
  m_info.removeAt(index);
  endRemoveRows();
}

void FileInfoModel::clear() {
  if (m_info.isEmpty()) {
    return;
  }

  beginRemoveRows(QModelIndex(), 0, m_info.size());
  m_info.clear();
  endRemoveRows();
}
