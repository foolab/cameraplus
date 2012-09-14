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
#include "exposure.h"
#include "aperture.h"
#include "iso.h"
#include "noisereduction.h"
#include "flickerreduction.h"
#include "mute.h"
#include "metadata.h"

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
  qmlRegisterType<Exposure>(URI, MAJOR, MINOR, "Exposure");
  qmlRegisterType<Aperture>(URI, MAJOR, MINOR, "Aperture");
  qmlRegisterType<Iso>(URI, MAJOR, MINOR, "Iso");
  qmlRegisterType<NoiseReduction>(URI, MAJOR, MINOR, "NoiseReduction");
  qmlRegisterType<FlickerReduction>(URI, MAJOR, MINOR, "FlickerReduction");
  qmlRegisterType<Mute>(URI, MAJOR, MINOR, "Mute");
  qmlRegisterType<MetaData>(URI, MAJOR, MINOR, "MetaData");

  qmlRegisterType<Mode>();
  qmlRegisterType<Capability>();

  engine->addImageProvider("preview", new PreviewProvider);
}
