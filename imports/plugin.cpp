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
#include "focus.h"
#include "autofocus.h"
#include "videomute.h"
#include "metadata.h"
#include "imagesettings.h"
#include "imageresolutionmodel.h"
#include "videosettings.h"
#include "videoresolutionmodel.h"
#include "notifications.h"
#include "sounds.h"

#include <QtDeclarative>

#define URI "QtCamera"
#define MAJOR 1
#define MINOR 0

void Plugin::registerTypes(QDeclarativeEngine *engine) {
  qmlRegisterType<Camera>(URI, MAJOR, MINOR, "Camera");
  qmlRegisterType<ImageMode>(URI, MAJOR, MINOR, "ImageMode");
  qmlRegisterType<VideoMode>(URI, MAJOR, MINOR, "VideoMode");

  qmlRegisterUncreatableType<Zoom>(URI, MAJOR, MINOR, "Zoom", QObject::tr("Cannot create separate instance of Zoom"));
  qmlRegisterUncreatableType<Flash>(URI, MAJOR, MINOR, "Flash", QObject::tr("Cannot create separate instance of Flash"));
  qmlRegisterUncreatableType<Scene>(URI, MAJOR, MINOR, "Scene", QObject::tr("Cannot create separate instance of Scene"));
  qmlRegisterUncreatableType<EvComp>(URI, MAJOR, MINOR, "EvComp", QObject::tr("Cannot create separate instance of EvComp"));
  qmlRegisterUncreatableType<WhiteBalance>(URI, MAJOR, MINOR, "WhiteBalance", QObject::tr("Cannot create separate instance of WhiteBalance"));
  qmlRegisterUncreatableType<ColorTone>(URI, MAJOR, MINOR, "ColorTone", QObject::tr("Cannot create separate instance of ColorTone"));
  qmlRegisterUncreatableType<Exposure>(URI, MAJOR, MINOR, "Exposure", QObject::tr("Cannot create separate instance of Exposure"));
  qmlRegisterUncreatableType<Aperture>(URI, MAJOR, MINOR, "Aperture", QObject::tr("Cannot create separate instance of Iso"));
  qmlRegisterUncreatableType<Iso>(URI, MAJOR, MINOR, "Iso", QObject::tr("Cannot create separate instance of Iso"));
  qmlRegisterUncreatableType<NoiseReduction>(URI, MAJOR, MINOR, "NoiseReduction", QObject::tr("Cannot create separate instance of NoiseReduction"));
  qmlRegisterUncreatableType<FlickerReduction>(URI, MAJOR, MINOR, "FlickerReduction", QObject::tr("Cannot create separate instance of FlickerReduction"));
  qmlRegisterUncreatableType<Focus>(URI, MAJOR, MINOR, "Focus", QObject::tr("Cannot create separate instance of Focus"));
  qmlRegisterUncreatableType<AutoFocus>(URI, MAJOR, MINOR, "AutoFocus", QObject::tr("Cannot create separate instance of AutoFocus"));

  qmlRegisterUncreatableType<VideoMute>(URI, MAJOR, MINOR, "VideoMute", QObject::tr("Cannot create separate instance of VideoMute"));
  qmlRegisterUncreatableType<VideoTorch>(URI, MAJOR, MINOR, "VideoTorch", QObject::tr("Cannot create separate instance of VideoTorch"));

  qmlRegisterType<MetaData>(URI, MAJOR, MINOR, "MetaData");
  qmlRegisterType<ImageSettings>(URI, MAJOR, MINOR, "ImageSettings");
  qmlRegisterType<VideoSettings>(URI, MAJOR, MINOR, "VideoSettings");
  qmlRegisterType<Sounds>(URI, MAJOR, MINOR, "Sounds");
  qmlRegisterInterface<Notifications>("Notifications");

  qmlRegisterUncreatableType<ImageResolutionModel>(URI, MAJOR, MINOR, "ImageResolutionModel",
			  "ImageResolutionModel can be obtained from ImageSettings");
  qmlRegisterUncreatableType<VideoResolutionModel>(URI, MAJOR, MINOR, "VideoResolutionModel",
			  "VideoResolutionModel can be obtained from VideoSettings");

  qmlRegisterType<Mode>();

  engine->addImageProvider("preview", new PreviewProvider);
}
