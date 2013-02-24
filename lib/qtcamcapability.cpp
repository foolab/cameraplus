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

#include "qtcamcapability.h"
#include "qtcamdevice.h"
#include "qtcamdevice_p.h"
#include "qtcamcapability_p.h"

#ifndef GST_USE_UNSTABLE_API
#define GST_USE_UNSTABLE_API
#endif
#include <gst/interfaces/photography.h>

QtCamCapability::Capabilities QtCamCapability::capabilities(QtCamDevice *dev) {
  if (!dev || !dev->d_ptr || !dev->d_ptr->videoSource) {
    return QtCamCapability::Capabilities(0);
  }

  if (!GST_IS_PHOTOGRAPHY(dev->d_ptr->videoSource)) {
    return QtCamCapability::Capabilities(0);
  }

  GstPhotography *photo = GST_PHOTOGRAPHY(dev->d_ptr->videoSource);
  return QtCamCapability::Capabilities(gst_photography_get_capabilities(photo));
}

QtCamCapability::QtCamCapability(QtCamCapabilityPrivate *d, QObject *parent) :
  QObject(parent), d_ptr(d) {
  d_ptr->bin = d_ptr->dev->d_ptr->cameraBin;
  d_ptr->src = d_ptr->dev->d_ptr->videoSource;
  d_ptr->q_ptr = this;

  if (!d_ptr->prop.isEmpty()) {
    d_ptr->startMonitoring();
  }
}

QtCamCapability::~QtCamCapability() {
  if (!d_ptr->prop.isEmpty()) {
    d_ptr->stopMonitoring();
  }

  delete d_ptr; d_ptr = 0;
}

bool QtCamCapability::isSupported() {
  return QtCamCapability::capabilities(d_ptr->dev).testFlag(d_ptr->cap);
}
