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

#include "qtcamgstsample.h"

class QtCamGstSamplePrivate {
public:
  GstBuffer *buffer;
  GstCaps *caps;
};

QtCamGstSample::QtCamGstSample(GstBuffer *buffer, GstCaps *caps) :
  d_ptr(new QtCamGstSamplePrivate) {

  d_ptr->buffer = gst_buffer_ref(buffer);
  d_ptr->caps = gst_caps_ref(caps);
}

QtCamGstSample::~QtCamGstSample() {
  gst_caps_unref(d_ptr->caps);
  gst_buffer_unref(d_ptr->buffer);

  delete d_ptr; d_ptr = 0;
}

GstBuffer *QtCamGstSample::buffer() const {
  return d_ptr->buffer;
}

GstCaps *QtCamGstSample::caps() const {
  return d_ptr->caps;
}

qint32 QtCamGstSample::width() const {
  const GstStructure *s = gst_caps_get_structure (d_ptr->caps, 0);
  qint32 w = -1;
  gst_structure_get_int (s, "width", &w);

  return w;
}

qint32 QtCamGstSample::height() const {
  const GstStructure *s = gst_caps_get_structure (d_ptr->caps, 0);
  qint32 h = -1;
  gst_structure_get_int (s, "height", &h);

  return h;
}
