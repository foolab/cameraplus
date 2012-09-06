#include "plugin.h"
#include "imagemode.h"
#include "videomode.h"
#include "camera.h"
#include "previewprovider.h"
#include "capability.h"
#include "zoom.h"
#include "flash.h"
#include "scene.h"
#include "evcomp.h"
#include "videotorch.h"
#include "whitebalance.h"
#include "colortone.h"

#include <QtDeclarative>

#define URI "QtCamera"
#define MAJOR 1
#define MINOR 0

void Plugin::registerTypes(QDeclarativeEngine *engine) {
  qmlRegisterType<Camera>(URI, MAJOR, MINOR, "Camera");
  qmlRegisterType<ImageMode>(URI, MAJOR, MINOR, "ImageMode");
  qmlRegisterType<VideoMode>(URI, MAJOR, MINOR, "VideoMode");
  qmlRegisterType<Zoom>(URI, MAJOR, MINOR, "Zoom");
  qmlRegisterType<Flash>(URI, MAJOR, MINOR, "Flash");
  qmlRegisterType<Scene>(URI, MAJOR, MINOR, "Scene");
  qmlRegisterType<EvComp>(URI, MAJOR, MINOR, "EvComp");
  qmlRegisterType<VideoTorch>(URI, MAJOR, MINOR, "VideoTorch");
  qmlRegisterType<WhiteBalance>(URI, MAJOR, MINOR, "WhiteBalance");
  qmlRegisterType<ColorTone>(URI, MAJOR, MINOR, "ColorTone");
  qmlRegisterType<Mode>();
  qmlRegisterType<Capability>();

  engine->addImageProvider("preview", new PreviewProvider);
}
