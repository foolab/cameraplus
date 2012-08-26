// -*- c++ -*-

#ifndef QT_CAM_META_DATA_H
#define QT_CAM_META_DATA_H

#include <QObject>

class QtCamMetaDataPrivate;
class QDateTime;
class QtCamDevice;

class QtCamMetaData : public QObject {
  Q_OBJECT

public:

  typedef enum {
    Landscape = 0,
    Portrait,
    InvertedLandscape,
    InvertedPortrait
  } Orientation;

  QtCamMetaData(QObject *parent = 0);
  ~QtCamMetaData();

  void setDevice(QtCamDevice *device);

  void setManufacturer(const QString& manufacturer);
  void setModel(const QString& model);
  void setCountry(const QString& country);
  void setCity(const QString& city);
  void setSuburb(const QString& suburb);
  void setLongitude(double longitude);
  void setLatitude(double latitude);
  void setElevation(double elevetion);
  void setOrientation(Orientation orientation);
  void setArtist(const QString& artist);
  void setDateTime(const QDateTime& dateTime);
  void setCaptureDirection(double direction);
  void setHorizontalError(double error);

  void reset();

private:
  QtCamMetaDataPrivate *d_ptr;
};

#endif /* QT_CAM_META_DATA_H */
