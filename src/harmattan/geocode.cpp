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

#include "geocode.h"
#include <QStringList>
#include <QDeclarativeInfo>

Geocode::Geocode(QObject *parent) :
  QObject(parent),
  m_service(0),
  m_id(0) {

}

Geocode::~Geocode() {
  setActive(false);
}

bool Geocode::isActive() const {
  return m_service != 0;
}

void Geocode::setActive(bool active) {
  if (active == Geocode::isActive()) {
    return;
  }

  if (!active) {
    delete m_service;
    m_service = 0;
    clear();
  }
  else {
    m_service = new GeoLocationService(this);
    QObject::connect(m_service, SIGNAL(addressQueryResult(uint, const GeoLocation&)),
		     this, SLOT(addressQueryResult(uint, const GeoLocation&)));
    QObject::connect(m_service, SIGNAL(addressQueryFinished(uint, GeoLocationService::GeoLocationError)),
		     this, SLOT(addressQueryFinished(uint, GeoLocationService::GeoLocationError)));

    m_service->init();
  }

  emit activeChanged();
}

QString Geocode::country() const {
  return m_country;
}

QString Geocode::city() const {
  return m_city;
}

QString Geocode::suburb() const {
  return m_suburb;
}

void Geocode::search(double longitude, double latitude) {
  if (!isActive()) {
    qmlInfo(this) << "geocoding is not active";
    return;
  }

  if (!m_service->isInitialized()) {
    m_pending = QGeoCoordinate(latitude, longitude);
    return;
  }

  m_id = 0;
  QGeoCoordinate coords(latitude, longitude);
  GeoLocationService::GeoLocationError err = m_service->addressQuery(coords, 1, &m_id);
  if (err == GeoLocationService::QueryFailedError) {
    setActive(false);
  }

  if (err != GeoLocationService::NoError) {
    qmlInfo(this) << "error" << err << "from GeoLocationService";
  }
}

void Geocode::clear() {
  m_id = 0;
  m_pending = QGeoCoordinate();

  if (!m_country.isEmpty()) {
    m_country.clear();
    emit countryChanged();
  }

  if (!m_city.isEmpty()) {
    m_city.clear();
    emit cityChanged();
  }

  if (!m_suburb.isEmpty()) {
    m_suburb.clear();
    emit suburbChanged();
  }
}

void Geocode::addressQueryResult(uint queryId, const GeoLocation& location) {
  if (m_id != queryId) {
    // We don't care about old queries.
    return;
  }

  if (m_country != location.address().country()) {
    m_country = location.address().country();
    emit countryChanged();
  }

  if (m_city != location.address().city()) {
    m_city = location.address().city();
    emit cityChanged();
  }

  if (m_suburb != location.address().district()) {
    m_suburb = location.address().district();
    emit suburbChanged();
  }
}

void Geocode::addressQueryFinished(uint queryId, GeoLocationService::GeoLocationError error) {
  if (m_id != queryId) {
    // We don't care about old queries.
    return;
  }

  if (error == GeoLocationService::QueryFailedError) {
    setActive(false);
  }

  if (error != GeoLocationService::NoError) {
    clear();
    qmlInfo(this) << "Error while geocoding" << error;
  }
}

void Geocode::initialized(bool success) {
  if (!success) {
    qmlInfo(this) << "Error initializing geocoding";
    setActive(false);
    return;
  }

  if (m_pending.isValid()) {
    search(m_pending.longitude(), m_pending.latitude());
    m_pending = QGeoCoordinate();
  }
}
