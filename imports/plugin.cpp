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
#include "imagesettings.h"
#include "imageresolutionmodel.h"
#include "videosettings.h"
#include "videoresolutionmodel.h"
#include "notifications.h"

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
  qmlRegisterType<ImageSettings>(URI, MAJOR, MINOR, "ImageSettings");
  qmlRegisterType<VideoSettings>(URI, MAJOR, MINOR, "VideoSettings");
  qmlRegisterInterface<Notifications>("Notifications");

  qmlRegisterUncreatableType<ImageResolutionModel>(URI, MAJOR, MINOR, "ImageResolutionModel",
			  "ImageResolutionModel can be obtained from ImageSettings");
  qmlRegisterUncreatableType<VideoResolutionModel>(URI, MAJOR, MINOR, "VideoResolutionModel",
			  "VideoResolutionModel can be obtained from VideoSettings");

  qmlRegisterType<Mode>();
  qmlRegisterType<Capability>();

  engine->addImageProvider("preview", new PreviewProvider);
}
