// -*- c++ -*-

#ifndef QT_CAM_PREVIEW_HELPER_H
#define QT_CAM_PREVIEW_HELPER_H

#include <QObject>


class QtCamPreviewHelper : public QObject {
  Q_OBJECT

public:
  QtCamPreviewHelper(QObject *parent = 0);
  ~QtCamPreviewHelper();

  void enable();
  void disable();
};

#endif /* QT_CAM_PREVIEW_HELPER_H */
