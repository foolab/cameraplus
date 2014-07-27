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

#include "roi.h"
#include "qtcamroi.h"
#include "qtcamdevice.h"
#include "qtcamviewfinder.h"

Roi::Roi(QtCamDevice *device, QObject *parent) :
  QObject(parent),
  m_roi(new QtCamRoi(device)) {

  QObject::connect(m_roi,
		   SIGNAL(regionsOfInterestUpdated(const QList<QRectF>&, const QRectF&, const QList<QRectF>&)),
		   this,
		   SLOT(handleRegionsChanged(const QList<QRectF>&, const QRectF&, const QList<QRectF>&)));
}

Roi::~Roi() {
  if (m_roi) {
    delete m_roi;
    m_roi = 0;
  }
}


void Roi::setEnabled(bool enabled) {
  if (Roi::isEnabled() != enabled) {
    if (m_roi) {
      m_roi->setEnabled(enabled);
      emit enabledChanged();
    }
  }
}

bool Roi::isEnabled() {
  return m_roi ? m_roi->isEnabled() : false;
}

void Roi::setRegionOfInterest(const QRectF& region) {
  if (!m_roi) {
    return;
  }

  QRectF area = m_roi->device()->viewfinder()->renderArea();

  QRectF rect(region.x() / area.width(),
	      region.y() / area.height(),
	      region.width() / area.width(),
	      region.height() / area.height());

  m_roi->setRegionOfInterest(rect);
}

void Roi::resetRegionOfInterest() {
  if (m_roi) {
    m_roi->resetRegionOfInterest();
  }
}

void Roi::handleRegionsChanged(const QList<QRectF>& regions, const QRectF& primary,
			       const QList<QRectF>& rest) {

  QRectF area = m_roi->device()->viewfinder()->renderArea();
  QVariantList regionsList;
  QVariantList restList;

  foreach (const QRectF& rect, regions) {
    regionsList << QRectF(rect.x() * area.width(),
			  rect.y() * area.height(),
			  rect.width() * area.width(),
			  rect.height() * area.height());
  }

  foreach (const QRectF& rect, rest) {
    restList << QRectF(rect.x() * area.width(),
		       rect.y() * area.height(),
		       rect.width() * area.width(),
		       rect.height() * area.height());
  }

  QVariant primaryRect = QVariant::fromValue(QRectF(primary.x() * area.width(),
						    primary.y() * area.height(),
						    primary.width() * area.width(),
						    primary.height() * area.height()));

  emit regionsChanged(regionsList, primaryRect, restList);
}

void Roi::prepareForDeviceChange() {
  if (m_roi) {
    delete m_roi;
    m_roi = 0;
  }
}
