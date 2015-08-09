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

#ifndef PANORAMA_INPUT_H
#define PANORAMA_INPUT_H

#include <QObject>
#include <stdint.h>

class QtCamGstSample;
class QSize;

class PanoramaInput : public QObject {
  Q_OBJECT

public:
  PanoramaInput(QObject *parent = 0);
  ~PanoramaInput();

signals:
  void dataAvailable(uint8_t *data, const QSize& size);

private slots:
#ifdef HARMATTAN
  void handleSample(const QtCamGstSample *sample);
#endif
};

#endif /* PANORAMA_INPUT_H */
