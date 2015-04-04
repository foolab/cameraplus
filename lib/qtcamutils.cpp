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

#include "qtcamutils.h"
#include <math.h>
#include <QSize>
#include <QHash>
#include <QMap>
#include <QDebug>

static inline uint qHash(const QSize& size) {
  return qHash(QString("%1x%2").arg(size.width()).arg(size.height()));
}

QString QtCamUtils::commonNameForResolution(const QSize& size) {
  static QHash<QSize, QString> names;

  if (names.isEmpty()) {
    names[QSize(1920, 1080)] = "1080p";
    names[QSize(1920, 1088)] = "1080p";
    names[QSize(1280, 720)] = "720p";
    names[QSize(640, 480)] = "VGA";
    names[QSize(848, 480)] = "WVGA"; // OK wikipedia lists it as wide PAL but WTH is wide PAL?
    names[QSize(800, 480)] = "WVGA";
    names[QSize(320, 240)] = "QVGA";
    names[QSize(720, 480)] = "480p";
    names[QSize(480, 320)] = "HVGA";
    names[QSize(352, 288)] = "VCD PAL";
    names[QSize(176, 144)] = "QCIF";
  }

  if (!names.contains(size)) {
    qWarning() << "commonName: unknown name for the resolution" << size;
    return QString("???");
  }

  return names[size];
}

QString QtCamUtils::aspectRatioForResolution(const QSize& size) {
  static QMap<float, QString> ratios;

  if (ratios.isEmpty()) {
    ratios[1.2] = "1:2";
    ratios[1.3] = "4:3";
    ratios[1.5] = "3:2";
    ratios[1.6] = "16:10";
    ratios[1.7] = "16:9";
    ratios[1.8] = "9:5";
  }

  float r = (size.width() * 1.0) / size.height();
  r = floor(r * 10) / 10.0;

  for (QMap<float, QString>::const_iterator iter = ratios.constBegin();
       iter != ratios.constEnd(); iter++) {
    if (qFuzzyCompare (r, iter.key())) {
      return iter.value();
    }
  }

  qWarning() << "calculateAspectRatio: unknown aspect ratio for size" << size;

  return QString("?:?");
}

float QtCamUtils::megapixelsForResolution(const QSize& size) {
  float mp = size.width() * size.height();
  mp = floor((mp * 10) / (1000.0 * 1000.0)) / 10;
  return mp;
}

QSize QtCamUtils::findMatchingResolution(const QSize& size, const QList<QSize>& sizes) {
  QString r = aspectRatioForResolution(size);
  if (r.isEmpty()) {
    return QSize();
  }

  QSize target;

  foreach (const QSize& s, sizes) {
    if (aspectRatioForResolution(s) != r) {
      continue;
    }

    int mp = s.width() * s.height();
    if (mp > target.width() * target.height()) {
      target = s;
    }
  }

  return target;
}
