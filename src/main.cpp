#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QtDeclarative>
#include <QGLWidget>

#include "imports/plugin.h"

#include "settings.h"

Q_DECL_EXPORT int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QDeclarativeView view;
  view.setViewport(new QGLWidget);
  view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
  view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

  Plugin::registerTypes(view.engine());
  qmlRegisterType<Settings>("CameraPlus", 1, 0, "Settings");

  QUrl sourceUrl = QUrl::fromLocalFile(QDir::currentPath() + "/main.qml");
  view.setSource(sourceUrl);

  view.showFullScreen();

  int ret = app.exec();
  return ret;
};
