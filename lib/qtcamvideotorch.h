// -*- c++ -*-

#ifndef QT_CAM_VIDEO_TORCH_H
#define QT_CAM_VIDEO_TORCH_H

#include <QObject>

class QtCamDevice;
class QtCamVideoTorchPrivate;

class QtCamVideoTorch : public QObject {
  Q_OBJECT

public:
  QtCamVideoTorch(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamVideoTorch();

  void setOn(bool on);
  bool isOn() const;

signals:
  void stateChanged();

private:
  QtCamVideoTorchPrivate *d_ptr;
};

#endif /* QT_CAM_VIDEO_TORCH_H */
