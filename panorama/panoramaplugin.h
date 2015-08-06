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

#ifndef PANORAMA_PLUGIN_H
#define PANORAMA_PLUGIN_H

#if defined(QT4)
#include <QDeclarativeExtensionPlugin>
#elif defined(QT5)
#include <QQmlExtensionPlugin>
#endif

#if defined(QT4)
class PanoramaPlugin : public QDeclarativeExtensionPlugin {
#elif defined(QT5)
class PanoramaPlugin : public QQmlExtensionPlugin {
#endif

  Q_OBJECT

#if defined(QT5)
  Q_PLUGIN_METADATA(IID "CameraPlus.Panorama");
#endif

public:
  PanoramaPlugin(QObject *parent = 0);
  ~PanoramaPlugin();

#if defined(QT4)
  void initializeEngine(QDeclarativeEngine *engine, const char *uri);
#elif defined(QT5)
  void initializeEngine(QQmlEngine *engine, const char *uri);
#endif

  void registerTypes(const char *uri);
};

#endif /* PANORAMA_PLUGIN_H */
