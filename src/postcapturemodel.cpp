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

#include "postcapturemodel.h"
#include <QSparqlConnection>
#include <QSparqlQuery>
#include <QSparqlResult>
#include <QSparqlError>
#include <QDeclarativeInfo>
#include <QDateTime>
#include <QDBusConnection>
#include <QStringList>
#include <QDBusMetaType>
#include <QDBusArgument>

#define DRIVER "QTRACKER_DIRECT"
#define QUERY "SELECT rdf:type(?urn) AS ?type nie:url(?urn) AS ?url nie:contentCreated(?urn) AS ?created nie:title(?urn) AS ?title nfo:fileName(?urn) AS ?filename nie:mimeType(?urn) AS ?mimetype tracker:available(?urn) AS ?available nfo:fileLastModified(?urn) as ?lastmod tracker:id(?urn) AS ?trackerid (EXISTS { ?urn nao:hasTag nao:predefined-tag-favorite }) AS ?favorite WHERE { ?urn nfo:equipment ?:equipment . {?urn a nfo:Video} UNION {?urn a nfo:Image}} ORDER BY DESC(?created)"

#define UPDATE_QUERY "SELECT rdf:type(?urn) AS ?type nie:url(?urn) AS ?url nie:contentCreated(?urn) AS ?created nie:title(?urn) AS ?title nfo:fileName(?urn) AS ?filename nie:mimeType(?urn) AS ?mimetype tracker:available(?urn) AS ?available nfo:fileLastModified(?urn) as ?lastmod tracker:id(?urn) AS ?trackerid (EXISTS { ?urn nao:hasTag nao:predefined-tag-favorite }) AS ?favorite WHERE {?urn a nfo:Visual . FILTER (tracker:id(?urn) IN (%1)) }"

#define TRACKER_SERVICE "org.freedesktop.Tracker1"
#define TRACKER_RESOURCE_PATH "/org/freedesktop/Tracker1/Resources"
#define TRACKER_RESOURCE_INTERFACE "org.freedesktop.Tracker1.Resources"
#define TRACKER_RESOURCE_SIGNAL "GraphUpdated"
#define PHOTO_CLASS "http://www.tracker-project.org/temp/nmm#Photo"
#define VIDEO_CLASS "http://www.tracker-project.org/temp/nmm#Video"
#define TRACKER_RESOURCE_SIGNAL_SIGNATURE "sa(iiii)a(iiii)"

class Quad {
public:
  int graph;
  int subject;
  int predicate;
  int object;
};

Q_DECLARE_METATYPE(Quad);
Q_DECLARE_METATYPE(QList<Quad>);

QDBusArgument& operator<<(QDBusArgument& argument, const Quad& t) {
  argument.beginStructure();
  argument << t.graph << t.subject << t.predicate << t.object;
  argument.endStructure();
  return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, Quad& t) {
  argument.beginStructure();
  argument >> t.graph >> t.subject >> t.predicate >> t.object;
  argument.endStructure();
  return argument;
}

PostCaptureModel::PostCaptureModel(QObject *parent) :
  QAbstractListModel(parent),
  m_connection(0) {

  QHash<int, QByteArray> roles;
  roles.insert(Item, "item");
  setRoleNames(roles);

  qDBusRegisterMetaType<Quad>();
  qDBusRegisterMetaType<QList<Quad> >();
}

PostCaptureModel::~PostCaptureModel() {
  qDeleteAll(m_items);
  m_items.clear();

  delete m_connection; m_connection = 0;
}

void PostCaptureModel::classBegin() {

}

void PostCaptureModel::componentComplete() {

}

void PostCaptureModel::reload() {
  delete m_connection; m_connection = 0;

  if (!m_items.isEmpty()) {
    beginRemoveRows(QModelIndex(), 0, m_items.size() - 1);
    qDeleteAll(m_items);
    m_items.clear();
    endRemoveRows();
  }

  m_connection = new QSparqlConnection(DRIVER, QSparqlConnectionOptions(), this);
  if (!m_connection->isValid()) {
    emit error(tr("Failed to create SPARQL connection"));
    return;
  }

  QString equipment = QString("urn:equipment:%1:%2:").arg(m_manufacturer).arg(m_model);

  QSparqlQuery q(QUERY, QSparqlQuery::SelectStatement);
  q.bindValue("equipment", equipment);
  exec(q);

  QDBusConnection::sessionBus().connect(TRACKER_SERVICE, TRACKER_RESOURCE_PATH,
					TRACKER_RESOURCE_INTERFACE, TRACKER_RESOURCE_SIGNAL,
					TRACKER_RESOURCE_SIGNAL_SIGNATURE,
					this, SLOT(graphUpdated(const QString&,
								const QList<Quad>&,
								const QList<Quad>&)));
}

void PostCaptureModel::exec(QSparqlQuery& query) {
  if (!m_connection) {
    qWarning() << "No connection to query";
    return;
  }

  QSparqlResult *result = m_connection->exec(query);

  if (result->hasError()) {
    QSparqlError error = result->lastError();
    qmlInfo(this) << "Error executing SPARQL query" << error.message();

    delete result;

    emit PostCaptureModel::error(tr("Failed to query tracker"));
  }

  if (result) {
    QObject::connect(result, SIGNAL(dataReady(int)), this, SLOT(dataReady(int)));
    QObject::connect(result, SIGNAL(finished()), result, SLOT(deleteLater()));
  }
}

int PostCaptureModel::rowCount(const QModelIndex& parent) const {
  if (parent.isValid()) {
    return 0;
  }

  return m_items.size();
}

QVariant PostCaptureModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size()) {
    return QVariant();
  }

  if (role == Item) {
    return QVariant::fromValue(dynamic_cast<QObject *>(m_items[index.row()]));
  }

  return QVariant();
}

QString PostCaptureModel::manufacturer() const {
  return m_manufacturer;
}

void PostCaptureModel::setManufacturer(const QString& manufacturer) {
  if (m_manufacturer != manufacturer) {
    m_manufacturer = manufacturer;
    emit manufacturerChanged();
  }
}

QString PostCaptureModel::model() const {
  return m_model;
}

void PostCaptureModel::setModel(const QString& model) {
  if (m_model != model) {
    m_model = model;
    emit modelChanged();
  }
}

void PostCaptureModel::dataReady(int totalCount) {
  Q_UNUSED(totalCount);

  QSparqlResult *result = dynamic_cast<QSparqlResult *>(sender());
  if (!result) {
    return;
  }

  while (result->next()) {
    addRow(new PostCaptureModelItem(result->current(), this));
  }

  result->previous();
}

void PostCaptureModel::addRow(PostCaptureModelItem *item) {
  if (m_hash.contains(item->trackerId())) {
    m_hash[item->trackerId()]->update(item);
    delete item;
    return;
  }

  beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
  m_items << item;
  m_hash.insert(item->trackerId(), item);

  endInsertRows();
}

void PostCaptureModel::remove(QObject *item) {
  PostCaptureModelItem *i = dynamic_cast<PostCaptureModelItem *>(item);
  if (!i) {
    qmlInfo(this) << "Invalid item to remove";
    return;
  }

  int index = m_items.indexOf(i);
  if (index == -1) {
    qmlInfo(this) << "Item" << i->trackerId() << "not found in model";
    return;
  }

  beginRemoveRows(QModelIndex(), index, index);
  m_items.takeAt(index);
  m_hash.remove(i->trackerId());
  endRemoveRows();

  i->deleteLater();
}

void PostCaptureModel::graphUpdated(const QString& className, const QList<Quad>& deleted,
				    const QList<Quad>& inserted) {

  // We will just assume tracker:available has changed and that's it.
  // We are not really interested in the rest of properties.

  if (!(className == QLatin1String(PHOTO_CLASS) || className == QLatin1String(VIDEO_CLASS))) {
    return;
  }

  QList<int> items;

  for (int x = 0; x < deleted.size(); x++) {
    Quad q = deleted[x];
    if (m_hash.contains(q.subject) && items.indexOf(q.subject) == -1) {
      items << q.subject;
    }
  }

  for (int x = 0; x < inserted.size(); x++) {
    Quad q = inserted[x];
    if (m_hash.contains(q.subject) && items.indexOf(q.subject) == -1) {
      items << q.subject;
    }
  }

  for (int x = 0; x < items.size(); x++) {
    QString query = QString(UPDATE_QUERY).arg(items[x]);
    QSparqlQuery q(query, QSparqlQuery::SelectStatement);

    exec(q);
  }
}

PostCaptureModelItem::PostCaptureModelItem(const QSparqlResultRow& row, QObject *parent) :
  QObject(parent) {

  for (int x = 0; x < row.count(); x++) {
    QSparqlBinding b = row.binding(x);
    m_data.insert(b.name(), b.value());
  }
}

QString PostCaptureModelItem::type() const {
  return value("type").toString();
}

QUrl PostCaptureModelItem::url() const {
  return value("url").toUrl();
}

QString PostCaptureModelItem::created() const {
  return formatDateTime(value("created").toDateTime());
}

QString PostCaptureModelItem::title() const {
  return value("title").toString();
}

QString PostCaptureModelItem::fileName() const {
  return value("filename").toString();
}

QString PostCaptureModelItem::mimeType() const {
  return value("mimetype").toString();
}

bool PostCaptureModelItem::available() const {
  return value("available", false).toBool();
}

QString PostCaptureModelItem::lastModified() const {
  return formatDateTime(value("lastmod").toDateTime());
}

unsigned PostCaptureModelItem::trackerId() const {
  return value("trackerid").toUInt();
}

bool PostCaptureModelItem::favorite() const {
  return value("favorite", false).toBool();
}

void PostCaptureModelItem::setFavorite(bool add) {
  if (favorite() != add) {
    m_data["favorite"] = add;

    emit favoriteChanged();
  }
}

QString PostCaptureModelItem::formatDateTime(const QDateTime& dt) const {
  return dt.toString();
}

void PostCaptureModelItem::update(PostCaptureModelItem *other) {
  // We will only update available, favorite and title:
#if 0
  qDebug() << "i" << trackerId() << other->trackerId()  << "\n"
	   << "a" << available() << other->available() << "\n"
	   << "t" << title() << other->title() << "\n"
	   << "f" << favorite() << other->favorite();
#endif

  if (available() != other->available()) {
    m_data["available"] = other->available();
    emit availableChanged();
  }

  if (title() != other->title()) {
    m_data["title"] = other->title();
    emit titleChanged();
  }

  if (favorite() != other->favorite()) {
    m_data["favorite"] = other->favorite();
    emit favoriteChanged();
  }
}

QVariant PostCaptureModelItem::value(const QString& id, const QVariant& def) const {
  return m_data.contains(id) ? m_data[id] : def;
}
