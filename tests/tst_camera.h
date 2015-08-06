#ifndef TST_CAMERA_H
#define TST_CAMERA_H

#include <QObject>
#include "qtcamviewfinder.h"
#include <gst/gst.h>

class Camera;

class tst_camera : public QObject {
  Q_OBJECT

public slots:
  void checkCapabilities();

private slots:
  void init();
  void cleanup();

  void reset();
  void startStop();
  void capabilities();

private:
  Camera *m_cam;
};

class TstViewfinder : public QtCamViewfinder {
public:
  TstViewfinder() {}
  ~TstViewfinder() { }

  GstElement *sinkElement() { return gst_element_factory_make("fakesink", NULL); }
  void start() {}
  void stop() {}

  QRectF renderArea() const { return QRectF(); }
  QSizeF videoResolution() const { return QSizeF(); }
  QtCamViewfinderRenderer *renderer() const { return 0; }
};

#endif /* TST_CAMERA_H */
