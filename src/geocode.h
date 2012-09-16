// -*- c++ -*-

#ifndef GEOCODE_H
#define GEOCODE_H

#include <QObject>
#include <QGeoSearchReply>
#include <QGeoServiceProvider>
#include <QGeoSearchManager>

using namespace QtMobility;

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

signals:
  void activeChanged();
  void countryChanged();
  void cityChanged();
  void suburbChanged();

private slots:
  void finished(QGeoSearchReply *reply);
  void error(QGeoSearchReply *reply, const QGeoSearchReply::Error& error,
	     const QString& errorString = QString());

private:
  void clear();

  QGeoServiceProvider *m_provider;
  QGeoSearchManager *m_manager;
  QGeoSearchReply *m_reply;

  bool m_active;
  QString m_country;
  QString m_city;
  QString m_suburb;
};

#endif /* GEOCODE_H */
