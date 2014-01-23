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

#include "qtcamroi.h"
#include "qtcamroi_p.h"

#ifndef G_VALUE_INIT
#define G_VALUE_INIT  { 0, { { 0 } } }
#endif /* G_VALUE_INIT */

QtCamRoi::QtCamRoi(QtCamDevice *dev, QObject *parent) :
  QObject(parent),
  d_ptr(new QtCamRoiPrivate(dev, this, this)) {

  d_ptr->init();
  d_ptr->installHandler();
}

QtCamRoi::~QtCamRoi() {
  delete d_ptr; d_ptr = 0;
}

void QtCamRoi::setRegionOfInterest(const QRectF& roi) {
  if (!d_ptr->dev || !d_ptr->dev->viewfinder()) {
    return;
  }

  QSizeF vf = d_ptr->dev->viewfinder()->videoResolution();
  if (vf.isEmpty()) {
    return;
  }

  int frameWidth = vf.width();
  int frameHeight = vf.height();
  int x = roi.x() * frameWidth;
  int y = roi.y() * frameHeight;
  int width = roi.width() * frameWidth;
  int height = roi.height() * frameHeight;

  // if we have an empty roi then we reset:
  int priority = roi.isEmpty() ? 0 : 1;

  GstStructure *region = gst_structure_new("region0",
					   "region-x", G_TYPE_UINT, x,
					   "region-y", G_TYPE_UINT, y,
					   "region-w", G_TYPE_UINT, width,
					   "region-h", G_TYPE_UINT, height,
					   "region-priority", G_TYPE_UINT, priority,
					   "region-id", G_TYPE_UINT, 0,
					   NULL);

  GValue regionValue = G_VALUE_INIT;
  GValue regionList = G_VALUE_INIT;

  g_value_init(&regionValue, GST_TYPE_STRUCTURE);
  g_value_init(&regionList, GST_TYPE_LIST);

  gst_value_set_structure(&regionValue, region);
  gst_value_list_append_value(&regionList, &regionValue);

  GstStructure *s = gst_structure_new("regions-of-interest",
				      "frame-width", G_TYPE_UINT, frameWidth,
				      "frame-height", G_TYPE_UINT, frameHeight,
				      NULL);
  gst_structure_set_value(s, "regions", &regionList);

  GstEvent *event = gst_event_new_custom(GST_EVENT_CUSTOM_UPSTREAM, s);
  gst_structure_free(region);
  g_value_unset(&regionValue);
  g_value_unset(&regionList);

  if (!d_ptr->sendEventToSource(event)) {
    qWarning() << "Failed to send ROI event";
  }
}

void QtCamRoi::resetRegionOfInterest() {
  setRegionOfInterest(QRectF());
}

void QtCamRoi::setEnabled(bool enabled) {
  if (enabled == isEnabled()) {
    return;
  }

  d_ptr->enabled = enabled;

  if (!d_ptr->roi) {
    return;
  }

  gboolean val = enabled ? TRUE : FALSE;

  g_object_set(d_ptr->roi, d_ptr->dev->config()->roiEnableProperty().toUtf8().constData(),
	       val, NULL);
}

bool QtCamRoi::isEnabled() {
  if (!d_ptr->roi) {
    return false;
  }

  gboolean val = FALSE;

  g_object_get(d_ptr->roi, d_ptr->dev->config()->roiEnableProperty().toUtf8().constData(),
	       &val, NULL);

  return val == TRUE;
}

QtCamDevice *QtCamRoi::device() {
  return d_ptr->dev;
}
