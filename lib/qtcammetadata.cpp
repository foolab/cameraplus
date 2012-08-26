#include "qtcammetadata.h"
#include <gst/gsttaglist.h>
#include "qtcamdevice.h"
#include "qtcamdevice_p.h"
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QDateTime>

const char *orientations[] = {
  "rotate-0",
  "rotate-90",
  "rotate-180",
  "rotate-270"
};

class QtCamMetaDataPrivate {
public:
  void addTag(const char *tag, const QString& value) {
    if (!setter) {
      return;
    }

    gst_tag_setter_add_tags(setter, GST_TAG_MERGE_REPLACE, tag, value.toUtf8().data(), NULL);
  }

  void addTag(const char *tag, double value) {
    if (!setter) {
      return;
    }

    gst_tag_setter_add_tags(setter, GST_TAG_MERGE_REPLACE, tag, value, NULL);
  }

  void addTag(const char *tag, GstDateTime *value) {
    if (!setter) {
      return;
    }

    gst_tag_setter_add_tags(setter, GST_TAG_MERGE_REPLACE, tag, value, NULL);
  }

  GstTagSetter *setter;
};

QtCamMetaData::QtCamMetaData(QObject *parent) :
  QObject(parent), d_ptr(new QtCamMetaDataPrivate) {
  d_ptr->setter = 0;
}

QtCamMetaData::~QtCamMetaData() {
  setDevice(0);
  delete d_ptr; d_ptr = 0;
}

void QtCamMetaData::setDevice(QtCamDevice *device) {
  if (d_ptr->setter) {
    gst_object_unref(d_ptr->setter);
    d_ptr->setter = 0;
  }

  if (!device || !device->d_ptr->cameraBin) {
    return;
  }

  if (!GST_IS_TAG_SETTER(device->d_ptr->cameraBin)) {
    return;
  }

  d_ptr->setter = GST_TAG_SETTER(gst_object_ref(device->d_ptr->cameraBin));
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
  if (d_ptr->setter) {
    gst_tag_setter_reset_tags(d_ptr->setter);
  }
}
