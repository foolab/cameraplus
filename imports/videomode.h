// -*- c++ -*-

#ifndef VIDEO_MODE_H
#define VIDEO_MODE_H

#include "mode.h"

class QtCamVideoMode;

class VideoMode : public Mode {
  Q_OBJECT
  Q_PROPERTY(bool recording READ isRecording NOTIFY recordingStateChanged);

public:
  VideoMode(QObject *parent = 0);
  ~VideoMode();

  Q_INVOKABLE bool startRecording(const QString& fileName);

  bool isRecording();

public slots:
  void stopRecording();

signals:
  void recordingStateChanged();

protected:
  virtual void preChangeMode();
  virtual void postChangeMode();
  virtual void changeMode();

private:
  QtCamVideoMode *m_video;
};

#endif /* VIDEO_MODE_H */
