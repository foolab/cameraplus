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

#include "panoramaplugin.h"
#include "panorama.h"
#include "panoramainput.h"
#if defined(QT4)
#include <QtDeclarative>
#elif defined(QT5)
#include <QtQuick>
#endif

#define MAJOR 1
#define MINOR 0

PanoramaPlugin::PanoramaPlugin(QObject *parent) :
#if defined(QT4)
  QDeclarativeExtensionPlugin(parent) {
#elif defined(QT5)
  QQmlExtensionPlugin(parent) {
#endif

}

PanoramaPlugin::~PanoramaPlugin() {

}

#if defined(QT4)
void PanoramaPlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri) {
#elif defined(QT5)
void PanoramaPlugin::initializeEngine(QQmlEngine *engine, const char *uri) {
#endif
  Q_UNUSED(uri);
  Q_UNUSED(engine);
}

void PanoramaPlugin::registerTypes(const char *uri) {
  Q_ASSERT(QLatin1String(uri) == QLatin1String("CameraPlus.Panorama"));

  qmlRegisterType<PanoramaInput>(uri, MAJOR, MINOR, "PanoramaInput");
  qmlRegisterType<Panorama>(uri, MAJOR, MINOR, "Panorama");

  qRegisterMetaType<Panorama::Error>("Panorama::Error");
}

#if defined(QT4)
Q_EXPORT_PLUGIN2(cameraplus-panorama, PanoramaPlugin);
#endif
