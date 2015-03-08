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

#include <QCoreApplication>
#include <qtcamera.h>
#include <qtcamdevice.h>
#include <qtcamconfig.h>
#include <qtcamnullviewfinder.h>
#include <qtcamutils.h>
#include <QDebug>

void dump(const QList<QSize>& res, const char *name) {
  qDebug() << name << "resolutions";

  foreach (const QSize& r, res) {
    qDebug() << r.width() << "x"<< r.height() <<
      QtCamUtils::aspectRatioForResolution(r) <<
      QtCamUtils::commonNameForResolution(r);
  }
}

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  QtCamera c;

  for (int x = 0; x < 2; x++) {
    QtCamDevice *dev = c.device(x, &c);
    dev->setViewfinder(new QtCamNullViewfinder(dev));

    if (!dev->start()) {
      qFatal("Failed to start camera");
    }

    dump(dev->queryImageResolutions(), "image");
    dump(dev->queryVideoResolutions(), "video");
    dump(dev->queryViewfinderResolutions(), "viewfinder");

    dev->stop(false);

    delete dev;
  }
}
