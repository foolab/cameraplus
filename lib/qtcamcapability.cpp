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
