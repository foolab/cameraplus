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

#include "panoramainput.h"
#include "qtcamgstsample.h"
#include "libyuv.h"
#include "panorama.h"
#include <QSize>

PanoramaInput::PanoramaInput(QObject *parent) :
  QObject(parent) {

}

PanoramaInput::~PanoramaInput() {

}

#ifdef HARMATTAN
void PanoramaInput::handleSample(const QtCamGstSample *sample) {
  QSize s(sample->width(), sample->height());

  GstBuffer *buffer = sample->buffer();
  int size = GST_BUFFER_SIZE(buffer);

  // Harmattan memory needs to be copied.
  guint8 src[size];
  memcpy(src, GST_BUFFER_DATA(buffer), size);

  guint8 *frame = new guint8[s.width() * s.height() * 3 / 2];

  guint8 *y = frame,
    *u = y + s.width() * s.height(),
    *v = u + s.width()/2 * s.height()/2;

  // No need for error checking because the function always returns 0
  libyuv::UYVYToI420(src, s.width() * 2,
		     y, s.width(),
		     u, s.width()/2,
		     v, s.width()/2,
		     s.width(), s.height());

  emit dataAvailable(frame, s);
}
#endif
