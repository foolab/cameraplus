#ifndef VIDEO_TORCH_H
#define VIDEO_TORCH_H

#include <QObject>

class Camera;
class QtCamVideoTorch;

class VideoTorch : public QObject {
  Q_OBJECT
  Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged);
  Q_PROPERTY(bool on READ isOn WRITE setOn NOTIFY stateChanged);

public:
  VideoTorch(QObject *parent = 0);
  ~VideoTorch();

  Camera *camera();
  void setCamera(Camera *camera);

  bool isOn() const;
  void setOn(bool on);

signals:
  void stateChanged();
  void cameraChanged();

private slots:
  void deviceChanged();

private:
  Camera *m_cam;
  QtCamVideoTorch *m_torch;
};

#endif /* VIDEO_TORCH_H */
