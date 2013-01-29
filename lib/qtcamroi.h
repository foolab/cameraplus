// -*- c++ -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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

#ifndef QT_CAM_ROI_H
#define QT_CAM_ROI_H

#include <QObject>

class QtCamDevice;
class QtCamRoiPrivate;
class QRectF;

class QtCamRoi : public QObject {
  Q_OBJECT
  friend class QtCamRoiPrivate;

public:
  QtCamRoi(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamRoi();

  void setRegionOfInterest(const QRectF& roi);
  void resetRegionOfInterest();

  void setEnabled(bool enabled);

  bool isEnabled();

signals:
  void regionsOfInterestUpdated(const QList<QRectF>& regions,
				const QRectF& primary,
				const QList<QRectF>& rest);

private:
  QtCamRoiPrivate *d_ptr;
};

#endif /* QT_CAM_ROI_H */
