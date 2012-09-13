// -*- c++ -*-

#ifndef QT_CAM_VIEWFINDER_H
#define QT_CAM_VIEWFINDER_H

#include <gst/gst.h>

class QtCamDevice;

class QtCamViewfinder {
public:
  QtCamViewfinder();
  virtual ~QtCamViewfinder();

  virtual GstElement *sinkElement() = 0;
  virtual bool setDevice(QtCamDevice *device) = 0;
  virtual void stop() = 0;
};

#endif /* QT_CAM_VIEWFINDER_H */
