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

#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <QAbstractListModel>
#include <QUrl>

class QDir;

class Plugin : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool valid READ isValid CONSTANT);
  Q_PROPERTY(QString uuid READ uuid CONSTANT);
  Q_PROPERTY(QString name READ name CONSTANT);
  Q_PROPERTY(QUrl icon READ icon CONSTANT);
  Q_PROPERTY(QUrl overlay READ overlay CONSTANT);
  Q_PROPERTY(QUrl settings READ settings CONSTANT);
  Q_PROPERTY(int mode READ mode CONSTANT);

public:
  Plugin(const QDir& dir, const QString& fileName, QObject * parent = 0);
  Plugin(QObject * parent = 0);
  ~Plugin();

  bool isValid() const;
  QString uuid() const;
  QString name() const;
  QUrl icon() const;
  QUrl overlay() const;
  QUrl settings() const;
  int mode() const;

private:
  QString m_uuid;
  QString m_name;
  QUrl m_icon;
  QUrl m_overlay;
  QUrl m_settings;
  int m_mode;
};

class PluginLoader : public QAbstractListModel {
  Q_OBJECT

public:
  PluginLoader(QObject * parent = 0);
  ~PluginLoader();

  typedef enum {
    PluginRole = Qt::UserRole + 1,
  } Roles;

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  Q_INVOKABLE Plugin *get(const QString& uuid);

private:
  void load();

  QList<Plugin *> m_items;

#if defined(QT5)
  QHash<int, QByteArray> roleNames() const;
  void setRoleNames(const QHash<int, QByteArray>& roles);
  QHash<int, QByteArray> m_roles;
#endif
};

#endif /* PLUGIN_LOADER_H */
