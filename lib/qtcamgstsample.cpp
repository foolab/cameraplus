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
#include <gst/gst.h>
#include <QDebug>

class QtCamGstSamplePrivate {
public:
  GstBuffer *buffer;
  GstCaps *caps;
  qint32 width;
  qint32 height;
  GstVideoFormat format;

#if GST_CHECK_VERSION(1,0,0)
  GstMapInfo info;
  bool mapped;
#endif
};

QtCamGstSample::QtCamGstSample(GstBuffer *buffer, GstCaps *caps) :
  d_ptr(new QtCamGstSamplePrivate) {

  d_ptr->buffer = gst_buffer_ref(buffer);
  d_ptr->caps = gst_caps_ref(caps);

#if GST_CHECK_VERSION(1,0,0)
  d_ptr->mapped = false;
#endif

  d_ptr->width = -1;
  d_ptr->height = -1;
  d_ptr->format = GST_VIDEO_FORMAT_UNKNOWN;

#if GST_CHECK_VERSION(1,0,0)
  GstVideoInfo info;
  if (!gst_video_info_from_caps (&info, d_ptr->caps)) {
    qCritical() << "Failed to parse GStreamer caps";
  } else {
    d_ptr->width = info.width;
    d_ptr->height = info.height;
    d_ptr->format = info.finfo->format;
  }
#else
  if (!gst_video_format_parse_caps (d_ptr->caps, &d_ptr->format, &d_ptr->width, &d_ptr->height)) {
    qCritical() << "Failed to parse GStreamer caps";
  }
#endif
}

QtCamGstSample::~QtCamGstSample() {
#if GST_CHECK_VERSION(1,0,0)
  if (d_ptr->mapped) {
    gst_buffer_unmap (d_ptr->buffer, &d_ptr->info);
    d_ptr->mapped = false;
  }
#endif
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
  return d_ptr->width;
}

qint32 QtCamGstSample::height() const {
  return d_ptr->height;
}

const uchar *QtCamGstSample::data() {
#if GST_CHECK_VERSION(1,0,0)
  if (!d_ptr->mapped) {
    if (!gst_buffer_map (d_ptr->buffer, &d_ptr->info, GST_MAP_READ)) {
      qCritical() << "Failed to map buffer";
      return NULL;
    }

    d_ptr->mapped = true;
  }

  return d_ptr->info.data;
#else
  return GST_BUFFER_DATA (d_ptr->buffer);
#endif
}

qint64 QtCamGstSample::size() {
#if GST_CHECK_VERSION(1,0,0)
  return gst_buffer_get_size (d_ptr->buffer);
#else
  return GST_BUFFER_SIZE (d_ptr->buffer);
#endif
}

