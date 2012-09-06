// -*- c++ -*-

#ifndef MODE_H
#define MODE_H

#include <QObject>

class Camera;
class QImage;
class QtCamMode;

class Mode : public QObject {
  Q_OBJECT

  Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged);
  Q_PROPERTY(bool canCapture READ canCapture NOTIFY canCaptureChanged);
  Q_PROPERTY(bool active READ isActive NOTIFY activeChanged);
  Q_PROPERTY(bool nightMode READ inNightMode WRITE setNightMode NOTIFY nightModeChanged);

public:
  Mode(QObject *parent = 0);
  virtual ~Mode();

  Camera *camera();
  virtual void setCamera(Camera *camera);

  bool isActive();

  bool canCapture();

  void setNightMode(bool night);
  bool inNightMode() const;

signals:
  void cameraChanged();
  void canCaptureChanged();
  void activeChanged();
  void previewAvailable(const QString& preview, const QString& fileName);
  void saved(const QString& fileName);
  void nightModeChanged();

private slots:
  void gotPreview(const QImage& image, const QString& fileName);

protected:
  virtual void preChangeMode() = 0;
  virtual void postChangeMode() = 0;
  virtual void changeMode() = 0;

  Camera *m_cam;
  QtCamMode *m_mode;

private slots:
  void deviceChanged();

private:
  unsigned long long m_seq;
};

#endif /* MODE_H */
