#include "imagemode.h"
#include "qtcamimagemode.h"
#include "qtcamdevice.h"
#include "camera.h"

ImageMode::ImageMode(QObject *parent) :
  Mode(parent),
  m_image(0) {

}

ImageMode::~ImageMode() {
  m_image = 0;
}


bool ImageMode::capture(const QString& fileName) {
  return m_image ? m_image->capture(fileName) : false;
}

void ImageMode::preChangeMode() {
  m_image = 0;
}

void ImageMode::postChangeMode() {
  m_image = m_cam->device()->imageMode();
}

void ImageMode::changeMode() {
  m_mode = m_cam->device()->imageMode();
}
