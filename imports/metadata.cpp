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

#include "metadata.h"
#include "camera.h"
#include <QDateTime>

MetaData::MetaData(QObject *parent) :
  QObject(parent),
  m_data(0),
  m_cam(0),
  m_longitude(0),
  m_latitude(0),
  m_elevation(0),
  m_orientation(MetaData::Unknown),
  m_captureDirection(0),
  m_horizontalError(0),
  m_longitudeValid(false),
  m_latitudeValid(false),
  m_elevationValid(false),
  m_captureDirectionValid(false),
  m_horizontalErrorValid(false),
  m_dateTimeEnabled(true) {

}

MetaData::~MetaData() {
  m_cam = 0;
}

Camera *MetaData::camera() const {
  return m_cam;
}

void MetaData::setCamera(Camera *camera) {
  if (m_cam) {
    QObject::disconnect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
  }

  m_cam = camera;

  if (m_cam) {
    QObject::connect(m_cam, SIGNAL(deviceChanged()), this, SLOT(deviceChanged()));
  }

  deviceChanged();

  emit cameraChanged();
}

QString MetaData::manufacturer() const {
  return m_manufacturer;
}

void MetaData::setManufacturer(const QString& manufacturer) {
  if (m_manufacturer != manufacturer) {
    m_manufacturer = manufacturer;
    emit manufacturerChanged();
  }
}

QString MetaData::model() const {
  return m_model;
}

void MetaData::setModel(const QString& model) {
  if (m_model != model) {
    m_model = model;
    emit modelChanged();
  }
}

QString MetaData::country() const {
  return m_country;
}

void MetaData::setCountry(const QString& country) {
  if (m_country != country) {
    m_country = country;
    emit countryChanged();
  }
}

QString MetaData::city() const {
  return m_city;
}

void MetaData::setCity(const QString& city) {
  if (m_city != city) {
    m_city = city;
    emit cityChanged();
  }
}

QString MetaData::suburb() const {
  return m_suburb;
}

void MetaData::setSuburb(const QString& suburb) {
  if (m_suburb != suburb) {
    m_suburb = suburb;
    emit suburbChanged();
  }
}

double MetaData::longitude() const {
  return m_longitude;
}

void MetaData::setLongitude(double longitude) {
  if (!qFuzzyCompare(m_longitude, longitude)) {
    m_longitude = longitude;
    emit longitudeChanged();
  }
}

double MetaData::latitude() const {
  return m_latitude;
}

void MetaData::setLatitude(double latitude) {
  if (!qFuzzyCompare(m_latitude, latitude)) {
    m_latitude = latitude;
    emit latitudeChanged();
  }
}

double MetaData::elevation() const {
  return m_elevation;
}

void MetaData::setElevation(double elevation) {
  if (!qFuzzyCompare(m_elevation, elevation)) {
    m_elevation = elevation;
    emit elevationChanged();
  }
}

MetaData::Orientation MetaData::orientation() const {
  return m_orientation;
}

void MetaData::setOrientation(const MetaData::Orientation& orientation) {
  if (m_orientation != orientation) {
    m_orientation = orientation;
    emit orientationChanged();
  }
}

QString MetaData::artist() const {
  return m_artist;
}

void MetaData::setArtist(const QString& artist) {
  // TODO: there is a limit for the number of bytes in the artist
  if (m_artist != artist) {
    m_artist = artist;
    emit artistChanged();
  }
}

int MetaData::captureDirection() const {
  return m_captureDirection;
}

void MetaData::setCaptureDirection(int captureDirection) {
  if (m_captureDirection != captureDirection) {
    m_captureDirection = captureDirection;
    emit captureDirectionChanged();
  }
}

double MetaData::horizontalError() const {
  return m_horizontalError;
}

void MetaData::setHorizontalError(double horizontalError) {
  if (!qFuzzyCompare(m_horizontalError, horizontalError)) {
    m_horizontalError = horizontalError;
    emit horizontalErrorChanged();
  }
}

void MetaData::setMetaData() {
  if (!m_data) {
    return;
  }

  m_data->reset();

  if (!m_manufacturer.isEmpty()) {
    m_data->setManufacturer(m_manufacturer);
  }

  if (!m_model.isEmpty()) {
    m_data->setModel(m_model);
  }

  if (!m_country.isEmpty()) {
    m_data->setCountry(m_country);
  }

  if (!m_city.isEmpty()) {
    m_data->setCity(m_city);
  }

  if (!m_suburb.isEmpty()) {
    m_data->setSuburb(m_suburb);
  }

  if (m_longitudeValid) {
    m_data->setLongitude(m_longitude);
  }

  if (m_latitudeValid) {
    m_data->setLatitude(m_latitude);
  }

  if (m_elevationValid) {
    m_data->setElevation(m_elevation);
  }

  if (m_orientation != MetaData::Unknown) {
    m_data->setOrientation((QtCamMetaData::Orientation)m_orientation);

    // TODO:
    //    qDebug() << m_orientation << m_captureDirection << m_captureDirectionValid;

    if (m_captureDirectionValid) {
      //      m_data->setCaptureDirection(m_captureDirection);
    }
  }

  if (!m_artist.isEmpty()) {
    m_data->setArtist(m_artist);
  }

  if (m_dateTimeEnabled) {
    m_data->setDateTime(QDateTime::currentDateTime());
  }

  if (m_horizontalErrorValid) {
    m_data->setHorizontalError(m_horizontalError);
  }
}

void MetaData::deviceChanged() {
  if (m_data) {
    delete m_data; m_data = 0;
  }

  m_data = new QtCamMetaData(this);
  m_data->setDevice(m_cam->device());
}

bool MetaData::isLongitudeValid() const {
  return m_longitudeValid;
}

void MetaData::setLongitudeValid(bool valid) {
  if (valid != m_longitudeValid) {
    m_longitudeValid = valid;
    emit longitudeValidChanged();
  }
}

bool MetaData::isLatitudeValid() const {
  return m_latitudeValid;
}

void MetaData::setLatitudeValid(bool valid) {
  if (valid != m_latitudeValid) {
    m_latitudeValid = valid;
    emit latitudeValidChanged();
  }
}

bool MetaData::isElevationValid() const {
  return m_elevationValid;
}

void MetaData::setElevationValid(bool valid) {
  if (valid != m_elevationValid) {
    m_elevationValid = valid;
    emit elevationValidChanged();
  }
}

bool MetaData::isCaptureDirectionValid() const {
  return m_captureDirectionValid;
}

void MetaData::setCaptureDirectionValid(bool valid) {
  if (valid != m_captureDirectionValid) {
    m_captureDirectionValid = valid;
    emit captureDirectionValidChanged();
  }
}

bool MetaData::isHorizontalErrorValid() const {
  return m_horizontalErrorValid;
}

void MetaData::setHorizontalErrorValid(bool valid) {
  if (valid != m_horizontalErrorValid) {
    m_horizontalErrorValid = valid;
    emit horizontalErrorValidChanged();
  }
}

bool MetaData::isDateTimeEnabled() const {
  return m_dateTimeEnabled;
}

void MetaData::setDateTimeEnabled(bool enabled) {
  if (m_dateTimeEnabled != enabled) {
    m_dateTimeEnabled = enabled;
    emit dateTimeEnabledChanged();
  }
}
