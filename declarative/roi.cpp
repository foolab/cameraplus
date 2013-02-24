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

#include "roi.h"
#include "qtcamroi.h"
#include "qtcamdevice.h"
#include "qtcamviewfinder.h"

class UnNormalizer {
public:
  void init(const QRectF& renderArea, bool normalize) {
    m_area = renderArea;
    m_normalize = normalize;
  }

  QRectF unNormalize(const QRectF& rect) {
    if (m_normalize) {
      return rect;
    }

    return QRectF(rect.x() * m_area.width(),
		  rect.y() * m_area.height(),
		  rect.width() * m_area.width(),
		  rect.height() * m_area.height());
  }

  QVariantList unNormalize(const QList<QRectF>& rects) {
    QVariantList vars;

    foreach (const QRectF& rect, rects) {
      vars.append(QVariant::fromValue(unNormalize(rect)));
    }

    return vars;
  }

private:
  QRectF m_area;
  bool m_normalize;
};

Roi::Roi(QtCamDevice *device, QObject *parent) :
  QObject(parent),
  m_roi(new QtCamRoi(device)),
  m_normalize(true) {

  QObject::connect(m_roi,
		   SIGNAL(regionsOfInterestUpdated(const QList<QRectF>&, const QRectF&, const QList<QRectF>&)),
		   this,
		   SLOT(handleRegionsChanged(const QList<QRectF>&, const QRectF&, const QList<QRectF>&)));
}

Roi::~Roi() {
  delete m_roi; m_roi = 0;
}


void Roi::setEnabled(bool enabled) {
  if (Roi::isEnabled() != enabled) {
    m_roi->setEnabled(enabled);
    emit enabledChanged();
  }
}

bool Roi::isEnabled() {
  return m_roi->isEnabled();
}

void Roi::setRegionOfInterest(const QRectF& region) {
  m_roi->setRegionOfInterest(region);
}

void Roi::resetRegionOfInterest() {
  m_roi->resetRegionOfInterest();
}

void Roi::handleRegionsChanged(const QList<QRectF>& regions, const QRectF& primary,
			       const QList<QRectF>& rest) {

  UnNormalizer n;

  n.init(m_roi->device()->viewfinder()->renderArea(), m_normalize);

  QVariantList regionsList = n.unNormalize(regions);
  QVariantList restList = n.unNormalize(rest);
  QVariant primaryRect = QVariant::fromValue(n.unNormalize(primary));

  emit regionsChanged(regionsList, primaryRect, restList);
}

bool Roi::normalize() const {
  return m_normalize;
}

void Roi::setNormalize(bool normalize) {
  if (normalize != m_normalize) {
    m_normalize = normalize;
    emit normalizeChanged();
  }
}
