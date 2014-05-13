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
#include <QDebug>
#include <QStringList>
#include <QGeoAddress>
#include <QQmlInfo>

Geocode::Geocode(QObject *parent) :
  QObject(parent),
  m_provider(0),
  m_manager(0),
  m_reply(0),
  m_active(false) {

  QStringList providers = QGeoServiceProvider::availableServiceProviders();
  int index = providers.indexOf("osm");

  if (index != -1) {
    m_provider = new QGeoServiceProvider(providers.at(index));
  }
  else if (!providers.isEmpty()) {
    m_provider = new QGeoServiceProvider(providers.at(0));
  }
  else {
    qCritical() << "Cannot find any geo-service providers";
    return;
  }

  if (!m_provider->geocodingFeatures().testFlag(QGeoServiceProvider::ReverseGeocodingFeature)) {
    qCritical() << "geo-search manager does not support reverse geocoding";

    m_manager = 0;
    return;
  }

  m_manager = m_provider->geocodingManager();
  if (!m_manager) {
    qCritical() << "Cannot get hold of the geocode manager" << m_provider->errorString();
    return;
  }

  QObject::connect(m_manager, SIGNAL(finished(QGeoCodeReply *)),
		   this, SLOT(finished(QGeoCodeReply *)));
  QObject::connect(m_manager, SIGNAL(error(QGeoCodeReply *, const QGeoCodeReply::Error&,
					   const QString&)),
		   this, SLOT(error(QGeoCodeReply *, const QGeoCodeReply::Error&,
				    const QString&)));
}

Geocode::~Geocode() {
  setActive(false);

  delete m_provider; m_provider = 0;
}

bool Geocode::isActive() const {
  return m_active;
}

void Geocode::setActive(bool active) {
  if (active == m_active) {
    return;
  }

  m_active = active;

  if (!m_active) {
    clear();
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
  if (!m_active || !m_provider) {
    return;
  }

  if (m_reply) {
    m_reply->abort();
    delete m_reply;
  }

  if (!m_manager) {
    qCritical() << "no geo-search manager";
    return;
  }

  m_reply = m_manager->reverseGeocode(QGeoCoordinate(latitude, longitude));
  if (!m_reply) {
    qCritical() << "geo-search manager provided a null reply!";
    return;
  }
}

void Geocode::clear() {
  if (m_reply) {
    m_reply->deleteLater();
    m_reply = 0;
  }

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

void Geocode::finished(QGeoCodeReply *reply) {
  if (reply->error() != QGeoCodeReply::NoError) {
    qmlInfo(this) << "Error while geocoding" << reply->errorString();

    reply->deleteLater();

    if (reply == m_reply) {
      if (reply->error() == QGeoCodeReply::CommunicationError) {
	// Network related error. We will disable ourselves for now.
	setActive(false);
      }

      m_reply = 0;
      clear();
    }

    return;
  }

  QList<QGeoLocation> places = reply->locations();
  if (!places.isEmpty()) {
    QGeoAddress address = places.at(0).address();
    if (m_country != address.country()) {
      m_country = address.country();
      emit countryChanged();
    }

    if (m_city != address.city()) {
      m_city = address.city();
      emit cityChanged();
    }

    if (m_suburb != address.district()) {
      m_suburb = address.district();
      emit suburbChanged();
    }
  }
  else {
    qmlInfo(this) << "No places found";
    clear();
  }

  reply->deleteLater();

  if (reply == m_reply) {
    m_reply = 0;
  }
}

void Geocode::error(QGeoCodeReply *reply, const QGeoCodeReply::Error& error,
		    const QString& errorString) {

  qmlInfo(this) << "Error while geocoding" << errorString;

  reply->deleteLater();

  if (reply == m_reply) {
    m_reply = 0;
    clear();

    if (error == QGeoCodeReply::CommunicationError) {
      // Network related error. We will disable ourselves for now.
      setActive(false);
    }
  }
}
