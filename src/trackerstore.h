// -*- c++ -*-

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

#ifndef TRACKER_STORE_H
#define TRACKER_STORE_H

#include <QObject>

class QSparqlConnection;
class QUrl;
class QSparqlQuery;

class TrackerStore : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged);
  Q_PROPERTY(QString manufacturer READ manufacturer WRITE setManufacturer NOTIFY manufacturerChanged);
  Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged);

public:
  TrackerStore(QObject *parent = 0);
  ~TrackerStore();

  bool isActive() const;
  void setActive(bool active);

  Q_INVOKABLE bool storeImage(const QString& path);
  Q_INVOKABLE bool storeVideo(const QString& path);

  QString manufacturer() const;
  void setManufacturer(const QString& manufacturer);

  QString model() const;
  void setModel(const QString& model);

  Q_INVOKABLE bool addToFavorites(const QUrl& url);
  Q_INVOKABLE bool removeFromFavorites(const QUrl& url);

signals:
  void activeChanged();
  void manufacturerChanged();
  void modelChanged();

private:
  bool execQuery(const QString& query, const QString& path);
  bool exec(QSparqlQuery& q);

  QSparqlConnection *m_connection;
  QString m_manufacturer;
  QString m_model;
};

#endif /* TRACKER_STORE_H */
