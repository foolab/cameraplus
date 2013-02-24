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

#ifndef ROI_H
#define ROI_H

#include <QObject>
#include <QRectF>
#include <QVariantList>

class QtCamDevice;
class QtCamRoi;

class Roi : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged);
  Q_PROPERTY(bool normalize READ normalize WRITE setNormalize NOTIFY normalizeChanged);

public:
  Roi(QtCamDevice *device, QObject *parent = 0);
  ~Roi();

  void setEnabled(bool enabled);
  bool isEnabled();

  QList<QRectF> regions();

  bool normalize() const;
  void setNormalize(bool normalize);

public slots:
  void setRegionOfInterest(const QRectF& region);
  void resetRegionOfInterest();

signals:
  void enabledChanged();
  void normalizeChanged();
  void regionsChanged(const QVariantList& regions, const QVariant& primary,
		      const QVariantList& rest);

private slots:
  void handleRegionsChanged(const QList<QRectF>& regions, const QRectF& primary,
			    const QList<QRectF>& rest);

private:
  QtCamRoi *m_roi;
  bool m_normalize;
};

#endif /* ROI_H */
