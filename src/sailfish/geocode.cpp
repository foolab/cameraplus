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

#include "geocode.h"
#include <QDebug>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QQmlInfo>
#include <QXmlStreamReader>

Geocode::Geocode(QObject *parent) :
  QObject(parent),
  m_manager(0),
  m_reply(0) {

}

Geocode::~Geocode() {
  setActive(false);
}

bool Geocode::isActive() const {
  return m_manager != 0;
}

void Geocode::setActive(bool active) {
  if (active == isActive()) {
    return;
  }

  if (active) {
    m_manager = new QNetworkAccessManager(this);
  } else {
    m_manager->deleteLater();
    m_manager = 0;

    if (m_reply) {
      m_reply->deleteLater();
      m_reply = 0;
    }
  }

  if (!active) {
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
  if (!isActive()) {
    return;
  }

  if (m_reply) {
    m_reply->abort();
    delete m_reply;
    m_reply = 0;
  }


  QUrl url(QStringLiteral("http://nominatim.openstreetmap.org/reverse"));
  QUrlQuery query;
  query.addQueryItem(QStringLiteral("format"), QStringLiteral("xml"));
  query.addQueryItem(QStringLiteral("accept-language"), QStringLiteral("en"));
  query.addQueryItem(QStringLiteral("lat"), QString::number(latitude));
  query.addQueryItem(QStringLiteral("lon"), QString::number(longitude));
  query.addQueryItem(QStringLiteral("zoom"), QStringLiteral("18"));
  query.addQueryItem(QStringLiteral("addressdetails"), QStringLiteral("1"));
  url.setQuery(query);

  m_reply = m_manager->get(QNetworkRequest(url));
  QObject::connect(m_reply, SIGNAL(finished()), this, SLOT(replyFinished()));
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

void Geocode::replyFinished() {
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
  if (!reply || reply != m_reply) {
    return;
  }

  if (reply->error() != QNetworkReply::NoError) {
    qmlInfo(this) << "Error while geocoding " << reply->errorString();

    // Network related error. We will disable ourselves for now.
    if (reply->error() != QNetworkReply::OperationCanceledError) {
      setActive(false);
    }

    reply->deleteLater();

    m_reply = 0;
    clear();

    return;
  }

  QXmlStreamReader reader(m_reply);
  while (!reader.atEnd()) {
    reader.readNext();
    if (reader.isStartElement()) {
      QStringRef name = reader.name();
      if (name == "suburb") {
	QString suburb = reader.readElementText();
	if (m_suburb != suburb) {
	  m_suburb = suburb;
	  emit suburbChanged();
	}
      } else if (name == "city") {
	QString city = reader.readElementText();
	if (m_city != city) {
	  m_city = city;
	  emit cityChanged();
	}
      } else if (name == "country") {
	QString country = reader.readElementText();
	if (m_country != country) {
	  m_country = country;
	  emit countryChanged();
	}
      }
    }
  }

  m_reply->deleteLater();
  m_reply = 0;
}
