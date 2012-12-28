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

#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QtDeclarative>
#include <QGLWidget>
#include <QuillFile>

#include "imports/plugin.h"

#include "settings.h"
#include "filenaming.h"
#include "quillitem.h"
#include "displaystate.h"
#include "fsmonitor.h"
#include "cameraresources.h"
#include "compass.h"
#include "orientation.h"
#include "geocode.h"
#include "mountprotector.h"
#include "trackerstore.h"
#include "focusrectangle.h"
#include "sharehelper.h"

static void initQuill() {
  // TODO: All these are hardcoded.
  Quill::setPreviewLevelCount(1);
  Quill::setPreviewSize(0, QSize(854, 480));
  Quill::setMinimumPreviewSize(0, QSize(854, 480));
  Quill::setThumbnailExtension("jpeg");
  Quill::setThumbnailFlavorName(0, "screen");
  Quill::setBackgroundRenderingColor(Qt::black);
  QString tempPath(QDir::homePath() +  QDir::separator() + ".config" +
		   QDir::separator() + "quill" + QDir::separator() + "tmp");
  QDir().mkpath(tempPath);
  Quill::setTemporaryFilePath(tempPath);
  Quill::setDBusThumbnailingEnabled(true);
  Quill::setThumbnailCreationEnabled(true);
}

Q_DECL_EXPORT int main(int argc, char *argv[]) {
  QApplication::setAttribute(Qt::AA_X11InitThreads, true);
  QApplication app(argc, argv);

  // Let's initialize Quill:
  initQuill();

  QDeclarativeView view;
  view.setViewport(new QGLWidget);
  view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
  view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

  Plugin::registerTypes(view.engine());
  qmlRegisterType<Settings>("CameraPlus", 1, 0, "Settings");
  qmlRegisterType<FileNaming>("CameraPlus", 1, 0, "FileNaming");
  qmlRegisterType<QuillItem>("CameraPlus", 1, 0, "QuillItem");
  qmlRegisterType<DisplayState>("CameraPlus", 1, 0, "DisplayState");
  qmlRegisterType<FSMonitor>("CameraPlus", 1, 0, "FSMonitor");
  qmlRegisterType<CameraResources>("CameraPlus", 1, 0, "CameraResources");
  qmlRegisterType<Compass>("CameraPlus", 1, 0, "Compass");
  qmlRegisterType<Orientation>("CameraPlus", 1, 0, "Orientation");
  qmlRegisterType<Geocode>("CameraPlus", 1, 0, "ReverseGeocode");
  qmlRegisterType<MountProtector>("CameraPlus", 1, 0, "MountProtector");
  qmlRegisterType<TrackerStore>("CameraPlus", 1, 0, "TrackerStore");
  qmlRegisterType<FocusRectangle>("CameraPlus", 1, 0, "FocusRectangle");
  qmlRegisterType<ShareHelper>("CameraPlus", 1, 0, "ShareHelper");

  QUrl sourceUrl = QUrl::fromLocalFile(QDir::currentPath() + "/main.qml");
  view.setSource(sourceUrl);

  view.showFullScreen();

  int ret = app.exec();
  return ret;
}
