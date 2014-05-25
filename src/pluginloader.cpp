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

#include "pluginloader.h"
#if defined(QT4)
#include <QDeclarativeInfo>
#include <QDeclarativeEngine>
#elif defined(QT5)
#include <QQmlInfo>
#include <QQmlEngine>
#endif
#include <QDir>
#include <QSettings>

#ifdef HARMATTAN
#define PLUGIN_PATH "/usr/share/cameraplus/modes/"
#else
#define PLUGIN_PATH "/usr/share/harbour-cameraplus/share/modes/"
#endif

Plugin::Plugin(const QDir& dir, const QString& fileName, QObject * parent) :
  QObject(parent) {
  QSettings settings(dir.absoluteFilePath(fileName), QSettings::IniFormat);
  m_uuid = settings.value("mode/uuid").toString();
  m_name = settings.value("mode/name").toString();
  m_icon = settings.value("mode/icon").toUrl();
  m_overlay = settings.value("mode/overlay").toUrl();
  m_settings = settings.value("mode/settings").toUrl();
  m_mode = settings.value("mode/mode").toInt();
  m_primarySupported = settings.value("mode/primary-camera", false).toBool();
  m_secondarySupported = settings.value("mode/secondary-camera", false).toBool();
}

Plugin::Plugin(QObject * parent) :
  QObject(parent),
  m_mode(0),
  m_primarySupported(false),
  m_secondarySupported(false) {

}

Plugin::~Plugin() {

}

bool Plugin::isValid() const {
  return !m_uuid.isEmpty() &&
    !m_name.isEmpty() &&
    m_icon.isValid() &&
    m_overlay.isValid() &&
    m_settings.isValid() &&
    (m_mode == 1 || m_mode == 2);
}

QString Plugin::uuid() const {
  return m_uuid;
}

QString Plugin::name() const {
  return m_name;
}

QUrl Plugin::icon() const {
  return m_icon;
}

QUrl Plugin::overlay() const {
  return m_overlay;
}

QUrl Plugin::settings() const {
  return m_settings;
}

int Plugin::mode() const {
  return m_mode;
}

bool Plugin::isPrimaryCameraSupported() const {
  return m_primarySupported;
}

bool Plugin::isSecondaryCameraSupported() const {
  return m_secondarySupported;
}

PluginLoader::PluginLoader(QObject *parent) :
  QAbstractListModel(parent) {

  QHash<int, QByteArray> roles;
  roles[PluginRole] = "plugin";

  setRoleNames(roles);

  load();
}

PluginLoader::~PluginLoader() {
  qDeleteAll(m_items);
  m_items.clear();
}

int PluginLoader::count() const {
  return rowCount(QModelIndex());
}

int PluginLoader::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid()) {
    return m_items.size();
  }

  return 0;
}

QVariant PluginLoader::data(const QModelIndex& index, int role) const {
  if (role == PluginRole && index.row() < m_items.size()) {
    QObject *o = m_items[index.row()];

#if defined(QT5)
    QQmlEngine::setObjectOwnership(o, QQmlEngine::CppOwnership);
#else
    QDeclarativeEngine::setObjectOwnership(o, QDeclarativeEngine::CppOwnership);
#endif

    return QVariant::fromValue<QObject *>(o);
  }

  return QVariant();
}

Plugin *PluginLoader::get(const QString& uuid) {
  foreach (Plugin *plugin, m_items) {
    if (plugin->uuid() == uuid) {
#if defined(QT5)
      QQmlEngine::setObjectOwnership(plugin, QQmlEngine::CppOwnership);
#else
      QDeclarativeEngine::setObjectOwnership(plugin, QDeclarativeEngine::CppOwnership);
#endif
      return plugin;
    }
  }

  return 0;
}

Plugin *PluginLoader::at(int pos) {
  if (pos >= m_items.size()) {
    return 0;
  }

  Plugin *plugin = m_items.at(pos);

#if defined(QT5)
      QQmlEngine::setObjectOwnership(plugin, QQmlEngine::CppOwnership);
#else
      QDeclarativeEngine::setObjectOwnership(plugin, QDeclarativeEngine::CppOwnership);
#endif

      return plugin;
}

void PluginLoader::load() {
  QDir dir(PLUGIN_PATH);

  QStringList entries = dir.entryList(QStringList() << "*.ini");

  foreach (const QString& entry, entries) {
    Plugin *plugin = new Plugin(dir, entry, this);

    if (!plugin->isValid()) {
      delete plugin;
    }
    else {
      m_items << plugin;
    }
  }
}

#if defined(QT5)
QHash<int, QByteArray> PluginLoader::roleNames() const {
  return m_roles;
}

void PluginLoader::setRoleNames(const QHash<int, QByteArray>& roles) {
  m_roles = roles;
}
#endif
