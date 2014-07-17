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

#include "trackerstore.h"
#include <QSparqlConnection>
#include <QSparqlQuery>
#include <QUrl>
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif
#include <QSparqlResult>
#include <QScopedPointer>
#include <QSparqlError>
#include <QDebug>
#include <QDateTime>

#define BEGIN_IMAGE "INSERT { _:x a nfo:Image, nmm:Photo"
#define BEGIN_VIDEO "INSERT { _:x a nfo:Video, nmm:Video"
#define QUERY_END ", nie:DataObject, nie:InformationElement, nfo:Media, nfo:Visual ; nie:url ?:file_url ; nfo:equipment ?:equipment^^xsd:string ; nie:contentCreated ?:contentCreated . }"

#define IMAGE_QUERY BEGIN_IMAGE QUERY_END
#define VIDEO_QUERY BEGIN_VIDEO QUERY_END

TrackerStore::TrackerStore(QObject *parent) :
  QObject(parent),
  m_connection(0) {

}

TrackerStore::~TrackerStore() {

}

bool TrackerStore::isActive() const {
  return m_connection != 0;
}

void TrackerStore::setActive(bool active) {
  if (isActive() == active) {
    return;
  }

  if (active) {
    m_connection = new QSparqlConnection("QTRACKER_DIRECT", QSparqlConnectionOptions(), this);
  }
  else {
    m_connection->deleteLater();
    m_connection = 0;
  }

  emit activeChanged();
}

QString TrackerStore::manufacturer() const {
  return m_manufacturer;
}

void TrackerStore::setManufacturer(const QString& manufacturer) {
  if (m_manufacturer != manufacturer) {
    m_manufacturer = manufacturer;
    emit manufacturerChanged();
  }
}

QString TrackerStore::model() const {
  return m_model;
}

void TrackerStore::setModel(const QString& model) {
  if (m_model != model) {
    m_model = model;
    emit modelChanged();
  }
}

bool TrackerStore::storeImage(const QString& path) {
  return execQuery(IMAGE_QUERY, path);
}

bool TrackerStore::storeVideo(const QString& path) {
  return execQuery(VIDEO_QUERY, path);
}

bool TrackerStore::execQuery(const QString& query, const QString& path) {
  QDateTime dateTime = QDateTime::currentDateTime();

  if (!isActive()) {
    qmlInfo(this) << "TrackerStore is not active";
    return false;
  }

  QString equipment = QString("urn:equipment:%1:%2:").arg(m_manufacturer).arg(m_model);

  QSparqlQuery q(query, QSparqlQuery::InsertStatement);
  q.bindValue("file_url", QUrl::fromLocalFile(path));
  q.bindValue("equipment", equipment);
  q.bindValue("contentCreated", dateTime.toString(Qt::ISODate) +
		  "." + QString().sprintf("%.3d", dateTime.time().msec()));

  return exec(q);
}

bool TrackerStore::exec(QSparqlQuery& q) {
  QScopedPointer<QSparqlResult> r(m_connection->syncExec(q));

  if (!r->hasError()) {
    return true;
  }

  while (r->next()) {
    // Nothing
  }

  qmlInfo(this) << "QtSparql error:" << r->lastError().message();

  return false;
}
