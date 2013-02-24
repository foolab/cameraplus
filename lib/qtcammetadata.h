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
