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

#include "qtcammetadata.h"
#include <gst/gsttaglist.h>
#include "qtcamdevice.h"
#include "qtcamdevice_p.h"
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QPointer>

const char *orientations[] = {
  "rotate-0",
  "rotate-90",
  "rotate-180",
  "rotate-270"
};

class QtCamMetaDataPrivate {
public:
  void addTag(const char *tag, const QString& value) {
    GstTagSetter *s = setter();
    if (!s) {
      return;
    }

    gst_tag_setter_add_tags(s, GST_TAG_MERGE_REPLACE, tag, value.toUtf8().data(), NULL);

    gst_object_unref(s);
  }

  void addTag(const char *tag, double value) {
    GstTagSetter *s = setter();
    if (!s) {
      return;
    }

    gst_tag_setter_add_tags(s, GST_TAG_MERGE_REPLACE, tag, value, NULL);

    gst_object_unref(s);
  }

  void addTag(const char *tag, GstDateTime *value) {
    GstTagSetter *s = setter();
    if (!s) {
      return;
    }

    gst_tag_setter_add_tags(s, GST_TAG_MERGE_REPLACE, tag, value, NULL);

    gst_object_unref(s);
  }

  GstTagSetter *setter() {
    if (!device || !device->d_ptr->cameraBin) {
      return 0;
    }

    if (!GST_IS_TAG_SETTER(device->d_ptr->cameraBin)) {
      return 0;
    }

    return GST_TAG_SETTER(gst_object_ref(device->d_ptr->cameraBin));
  }

  QPointer<QtCamDevice> device;
};

QtCamMetaData::QtCamMetaData(QObject *parent) :
  QObject(parent), d_ptr(new QtCamMetaDataPrivate) {
}

QtCamMetaData::~QtCamMetaData() {
  setDevice(0);
  delete d_ptr; d_ptr = 0;
}

void QtCamMetaData::setDevice(QtCamDevice *device) {
  if (device != d_ptr->device) {
    d_ptr->device = device;
  }
}

void QtCamMetaData::setManufacturer(const QString& manufacturer) {
  d_ptr->addTag(GST_TAG_DEVICE_MANUFACTURER, manufacturer);
}

void QtCamMetaData::setModel(const QString& model) {
  d_ptr->addTag(GST_TAG_DEVICE_MODEL, model);
}

void QtCamMetaData::setCountry(const QString& country) {
  d_ptr->addTag(GST_TAG_GEO_LOCATION_COUNTRY, country);
}

void QtCamMetaData::setCity(const QString& city) {
  d_ptr->addTag(GST_TAG_GEO_LOCATION_CITY, city);
}

void QtCamMetaData::setSuburb(const QString& suburb) {
  d_ptr->addTag(GST_TAG_GEO_LOCATION_SUBLOCATION, suburb);
}

void QtCamMetaData::setLongitude(double longitude) {
  d_ptr->addTag(GST_TAG_GEO_LOCATION_LONGITUDE, longitude);
}

void QtCamMetaData::setLatitude(double latitude) {
  d_ptr->addTag(GST_TAG_GEO_LOCATION_LATITUDE, latitude);
}

void QtCamMetaData::setElevation(double elevation) {
  d_ptr->addTag(GST_TAG_GEO_LOCATION_ELEVATION, elevation);
}

void QtCamMetaData::setOrientation(Orientation orientation) {
  int len = sizeof(orientations) / sizeof(orientations[0]);

  if (orientation <= 0 || orientation >= len) {
    orientation = Landscape;
  }

  d_ptr->addTag(GST_TAG_IMAGE_ORIENTATION, orientations[orientation]);
}

void QtCamMetaData::setArtist(const QString& artist) {
  d_ptr->addTag(GST_TAG_ARTIST, artist);
}

void QtCamMetaData::setDateTime(const QDateTime& dateTime) {
  QDate d = dateTime.date();
  QTime t = dateTime.time();

  int day = d.day();
  int month = d.month();
  int year = d.year();
  int hour = t.hour();
  int minute = t.minute();

  // GstDateTime seconds expects microseconds to be there too :|
  gdouble seconds = t.second();
  seconds += t.msec()/(1000.0);

  // Current UTC time. Created through string so that the link between
  // current and utc is lost and secsTo returns non-zero values.
  QDateTime utcTime = QDateTime::fromString(dateTime.toUTC().toString());
  gfloat tzoffset = (utcTime.secsTo(dateTime)/3600.0);

  GstDateTime *dt = gst_date_time_new(tzoffset, year, month, day,
				      hour, minute, seconds);

  d_ptr->addTag(GST_TAG_DATE_TIME, dt);

  gst_date_time_unref(dt);
}

void QtCamMetaData::setCaptureDirection(double direction) {
  d_ptr->addTag(GST_TAG_GEO_LOCATION_CAPTURE_DIRECTION, direction);
}

void QtCamMetaData::setHorizontalError(double error) {
  d_ptr->addTag(GST_TAG_GEO_LOCATION_HORIZONTAL_ERROR, error);
}

void QtCamMetaData::reset() {
  GstTagSetter *s = d_ptr->setter();
  if (!s) {
    return;
  }

  gst_tag_setter_reset_tags(s);

  gst_object_unref(s);
}
