// -*- c++ -*-

#ifndef META_DATA_H
#define META_DATA_H

#include <QObject>
#include "qtcammetadata.h"

class Camera;

class MetaData : public QObject {
  Q_OBJECT

  Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged);
  Q_PROPERTY(QString manufacturer READ manufacturer WRITE setManufacturer NOTIFY manufacturerChanged);
  Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged);
  Q_PROPERTY(QString country READ country WRITE setCountry NOTIFY countryChanged);
  Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged);
  Q_PROPERTY(QString suburb READ suburb WRITE setSuburb NOTIFY suburbChanged);
  Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged);
  Q_PROPERTY(bool longitudeValid READ isLongitudeValid WRITE setLongitudeValid NOTIFY longitudeValidChanged);
  Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged);
  Q_PROPERTY(bool latitudeValid READ isLatitudeValid WRITE setLatitudeValid NOTIFY latitudeValidChanged);
  Q_PROPERTY(double elevation READ elevation WRITE setElevation NOTIFY elevationChanged);
  Q_PROPERTY(bool elevationValid READ isElevationValid WRITE setElevationValid NOTIFY elevationValidChanged);
  Q_PROPERTY(Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged);
  Q_PROPERTY(QString artist READ artist WRITE setArtist NOTIFY artistChanged);
  Q_PROPERTY(int captureDirection READ captureDirection WRITE setCaptureDirection NOTIFY captureDirectionChanged);
  Q_PROPERTY(bool captureDirectionValid READ isCaptureDirectionValid WRITE setCaptureDirectionValid NOTIFY captureDirectionValidChanged);
  Q_PROPERTY(double horizontalError READ horizontalError WRITE setHorizontalError NOTIFY horizontalErrorChanged);
  Q_PROPERTY(bool horizontalErrorValid READ isHorizontalErrorValid WRITE setHorizontalErrorValid NOTIFY horizontalErrorValidChanged);
  Q_PROPERTY(bool dateTimeEnabled READ isDateTimeEnabled WRITE setDateTimeEnabled NOTIFY dateTimeEnabledChanged);
  Q_ENUMS(Orientation);

public:
  typedef enum {
    Unknown = -1,
    Landscape = QtCamMetaData::Landscape,
    Portrait = QtCamMetaData::Portrait,
    InvertedLandscape = QtCamMetaData::InvertedLandscape,
    InvertedPortrait = QtCamMetaData::InvertedPortrait
  } Orientation;

  MetaData(QObject *parent = 0);
  ~MetaData();

  Camera *camera() const;
  void setCamera(Camera *camera);

  QString manufacturer() const;
  void setManufacturer(const QString& manufacturer);

  QString model() const;
  void setModel(const QString& model);

  QString country() const;
  void setCountry(const QString& country);

  QString city() const;
  void setCity(const QString& city);

  QString suburb() const;
  void setSuburb(const QString& suburb);

  double longitude() const;
  void setLongitude(double longitude);

  double latitude() const;
  void setLatitude(double latitude);

  double elevation() const;
  void setElevation(double elevation);

  Orientation orientation() const;
  void setOrientation(const Orientation& orientation);

  QString artist() const;
  void setArtist(const QString& artist);

  int captureDirection() const;
  void setCaptureDirection(int captureDirection);

  double horizontalError() const;
  void setHorizontalError(double horizontalError);

  bool isLongitudeValid() const;
  void setLongitudeValid(bool valid);

  bool isLatitudeValid() const;
  void setLatitudeValid(bool valid);

  bool isElevationValid() const;
  void setElevationValid(bool valid);

  bool isCaptureDirectionValid() const;
  void setCaptureDirectionValid(bool valid);

  bool isHorizontalErrorValid() const;
  void setHorizontalErrorValid(bool valid);

  bool isDateTimeEnabled() const;
  void setDateTimeEnabled(bool enabled);

public slots:
  void setMetaData();

signals:
  void cameraChanged();
  void manufacturerChanged();
  void modelChanged();
  void countryChanged();
  void cityChanged();
  void suburbChanged();
  void longitudeChanged();
  void latitudeChanged();
  void elevationChanged();
  void orientationChanged();
  void artistChanged();
  void dateTimeChanged();
  void captureDirectionChanged();
  void horizontalErrorChanged();
  void longitudeValidChanged();
  void latitudeValidChanged();
  void elevationValidChanged();
  void captureDirectionValidChanged();
  void horizontalErrorValidChanged();
  void dateTimeEnabledChanged();

private slots:
  void deviceChanged();

private:
  QtCamMetaData *m_data;
  Camera *m_cam;
  QString m_manufacturer;
  QString m_model;
  QString m_country;
  QString m_city;
  QString m_suburb;
  double m_longitude;
  double m_latitude;
  double m_elevation;
  Orientation m_orientation;
  QString m_artist;
  int m_captureDirection;
  double m_horizontalError;
  bool m_longitudeValid;
  bool m_latitudeValid;
  bool m_elevationValid;
  bool m_captureDirectionValid;
  bool m_horizontalErrorValid;
  bool m_dateTimeEnabled;
};

#endif /* META_DATA_H */
