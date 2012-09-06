#include "videomode.h"
#include "qtcamvideomode.h"
#include "qtcamdevice.h"
#include "camera.h"

VideoMode::VideoMode(QObject *parent) :
  Mode(parent),
  m_video(0) {

}

VideoMode::~VideoMode() {
  m_video = 0;
}


bool VideoMode::startRecording(const QString& fileName) {
  return m_video ? m_video->startRecording(fileName) : false;
}

void VideoMode::stopRecording() {
  if (m_video) {
    m_video->stopRecording();
  }
}

void VideoMode::preChangeMode() {
  if (m_video) {
    QObject::disconnect(m_video, SIGNAL(recordingStateChanged()),
			this, SIGNAL(recordingStateChanged()));
  }

  m_video = 0;
}

void VideoMode::postChangeMode() {
  m_video = m_cam->device()->videoMode();

  if (m_video) {
    QObject::connect(m_video, SIGNAL(recordingStateChanged()),
			this, SIGNAL(recordingStateChanged()));
  }
}

bool VideoMode::isRecording() {
  return m_video ? m_video->isRecording() : false;
}

void VideoMode::changeMode() {
  m_mode = m_cam->device()->videoMode();
}
