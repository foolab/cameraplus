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

#include "qtcamautofocus.h"
#include "qtcamautofocus_p.h"
#include <QRectF>

#ifndef G_VALUE_INIT
#define G_VALUE_INIT  { 0, { { 0 } } }
#endif /* G_VALUE_INIT */

QtCamAutoFocus::QtCamAutoFocus(QtCamDevice *dev, QObject *parent) :
  QObject(parent),
  d_ptr(new QtCamAutoFocusPrivate(dev, this, this)) {

}

QtCamAutoFocus::~QtCamAutoFocus() {
  delete d_ptr; d_ptr = 0;
}

QtCamAutoFocus::Status QtCamAutoFocus::status() {
  return d_ptr->status;
}

QtCamAutoFocus::Status QtCamAutoFocus::cafStatus() {
  return d_ptr->cafStatus;
}

bool QtCamAutoFocus::startAutoFocus() {
  return d_ptr->setEnabled(true);
}

bool QtCamAutoFocus::stopAutoFocus() {
  return d_ptr->setEnabled(false);
}

bool QtCamAutoFocus::canFocus(const QtCamScene::SceneMode& mode) {
  if (mode == QtCamScene::Landscape || mode == QtCamScene::Sport) {
    return false;
  }

  return true;
}

void QtCamAutoFocus::setRegionOfInterest(const QRectF& roi) {
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
