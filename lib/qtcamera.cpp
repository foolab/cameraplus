/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
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

#include "qtcamera.h"
#include "qtcamscanner.h"
#include "qtcamconfig.h"
#include "qtcamdevice.h"
#include "gst/gstcopy.h"
#include <gst/gst.h>

class QtCameraPrivate {
public:
  QtCameraPrivate() {
    qt_cam_copy_register();
  }

  QtCamConfig *conf;
  QtCamScanner *scanner;
};

QtCamera::QtCamera(QObject *parent) :
  QObject(parent), d_ptr(new QtCameraPrivate) {

  gst_init(0, 0);

  d_ptr->conf = new QtCamConfig(this);
  d_ptr->scanner = new QtCamScanner(d_ptr->conf, this);

  refreshDevices();
}

QtCamera::QtCamera(const QString& configPath, QObject *parent) :
  QObject(parent), d_ptr(new QtCameraPrivate) {

  gst_init(0, 0);

  d_ptr->conf = new QtCamConfig(configPath, this);
  d_ptr->scanner = new QtCamScanner(d_ptr->conf, this);

  refreshDevices();
}

QtCamera::QtCamera(QtCamConfig *config, QObject *parent) :
  QObject(parent), d_ptr(new QtCameraPrivate) {

  gst_init(0, 0);

  d_ptr->conf = config;
  d_ptr->scanner = new QtCamScanner(d_ptr->conf, this);

  refreshDevices();
}

QtCamera::~QtCamera() {
  delete d_ptr; d_ptr = 0;

  gst_deinit();
}

void QtCamera::refreshDevices() {
  d_ptr->scanner->refresh();
}

QList<QPair<QString, QVariant> > QtCamera::devices() const {
  return d_ptr->scanner->devices();
}

QtCamDevice *QtCamera::device(const QVariant& id, QObject *parent) {
  QList<QPair<QString, QVariant> > devs = devices();

  // G++ barfs with foreach and class templates.
  typedef QPair<QString, QVariant> Dev;
  foreach (const Dev& dev, devs) {
    if (dev.second == id) {
      return new QtCamDevice(d_ptr->conf, dev.first, dev.second, parent ? parent : this);
    }
  }

  return 0;
}

QtCamConfig *QtCamera::config() const {
  return d_ptr->conf;
}
