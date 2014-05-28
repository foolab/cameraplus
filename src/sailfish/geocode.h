// -*- c++ -*-

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

#ifndef GEOCODE_H
#define GEOCODE_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

class Geocode : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged);
  Q_PROPERTY(QString country READ country NOTIFY countryChanged);
  Q_PROPERTY(QString city READ city NOTIFY cityChanged);
  Q_PROPERTY(QString suburb READ suburb NOTIFY suburbChanged);

public:
  Geocode(QObject *parent = 0);
  ~Geocode();

  bool isActive() const;
  void setActive(bool active);

  QString country() const;
  QString city() const;
  QString suburb() const;

public slots:
  void search(double longitude, double latitude);
  void clear();

signals:
  void activeChanged();
  void countryChanged();
  void cityChanged();
  void suburbChanged();

private slots:
  void replyFinished();

private:
  QNetworkAccessManager *m_manager;
  QNetworkReply *m_reply;

  QString m_country;
  QString m_city;
  QString m_suburb;
};

#endif /* GEOCODE_H */
