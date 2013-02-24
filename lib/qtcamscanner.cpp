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

#include "qtcamscanner.h"
#include "qtcamconfig.h"
#include <QDir>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <gst/gst.h>

class QtCamScannerPrivate {
public:
  void scanEnum();
  void scanV4l2();

  QtCamConfig *conf;
  QList<QPair<QString, QVariant> > devices;
};

void QtCamScannerPrivate::scanEnum() {
  // Too bad there's no way to get the values of an enum without creating the element :(
  GstElement *elem = gst_element_factory_make(conf->videoSource().toAscii(), NULL);
  if (!elem) {
    return;
  }

  GParamSpec *spec = g_object_class_find_property(G_OBJECT_GET_CLASS(elem),
						  conf->deviceScannerProperty().toAscii());
  if (!spec) {
    gst_object_unref(elem);
    return;
  }

  if (!G_IS_PARAM_SPEC_ENUM(spec)) {
    gst_object_unref(elem);
    return;
  }

  GParamSpecEnum *e = G_PARAM_SPEC_ENUM(spec);
  // First add the default:
  devices << qMakePair<QString, QVariant>(e->enum_class->values[e->default_value].value_name,
					  QByteArray::number(e->default_value));

  for (int x = e->enum_class->minimum; x <= e->enum_class->maximum; x++) {
    if (x != e->default_value) {
      devices << qMakePair<QString, QVariant>(e->enum_class->values[x].value_name,
					      QByteArray::number(x));
    }
  }

  gst_object_unref(elem);
}

void QtCamScannerPrivate::scanV4l2() {
  QDir d("/dev/", "video?", QDir::Name | QDir::IgnoreCase, QDir::System);

  QStringList entries = d.entryList();

  foreach (const QString& dv, entries) {
    QString dev = d.absoluteFilePath(dv);
    struct v4l2_capability cap;
    memset(&cap, 0x0, sizeof(cap));

    int fd = open(dev.toLocal8Bit().constData(), O_RDONLY);
    if (fd == -1) {
      continue;
    }

    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) != 0) {
      close(fd);
      continue;
    }

    close(fd);

    if (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) {
      devices << qMakePair<QString, QVariant>((char *)cap.card, dev.toLocal8Bit());
    }
  }
}

QtCamScanner::QtCamScanner(QtCamConfig *conf, QObject *parent) :
  QObject(parent), d_ptr(new QtCamScannerPrivate) {

  d_ptr->conf = conf;
}

QtCamScanner::~QtCamScanner() {
  delete d_ptr; d_ptr = 0;
}

void QtCamScanner::refresh() {
  d_ptr->devices.clear();

  if (d_ptr->conf->deviceScannerType() == SCANNER_TYPE_ENUM) {
    d_ptr->scanEnum();
  }
  else {
    d_ptr->scanV4l2();
  }
}

QList<QPair<QString, QVariant> > QtCamScanner::devices() const {
  return d_ptr->devices;
}
