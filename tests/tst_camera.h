#ifndef TST_CAMERA_H
#define TST_CAMERA_H

#include "qtcamviewfinder.h"
#include <gst/gst.h>

class TstViewfinder : public QtCamViewfinder {
public:
  TstViewfinder() {}
  ~TstViewfinder() { }

  GstElement *sinkElement() { return gst_element_factory_make("fakesink", NULL); }
  void start() {}
  void stop() {}

  QRectF renderArea() const { return QRectF(); }
  QSizeF videoResolution() const { return QSizeF(); }
};

#endif /* TST_CAMERA_H */
