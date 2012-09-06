#include "previewprovider.h"

PreviewProvider *PreviewProvider::m_instance = 0;

PreviewProvider::PreviewProvider() :
  QDeclarativeImageProvider(QDeclarativeImageProvider::Image) {

  m_instance = this;
}

PreviewProvider::~PreviewProvider() {
  m_instance = 0;
}

QImage PreviewProvider::requestImage(const QString& id, QSize *size, const QSize& requestedSize) {
  QMutexLocker lock(&m_mutex);

  QImage res = m_image;

  if (!requestedSize.isEmpty()) {
    res = res.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  }

  if (size) {
    *size = res.size();
  }

  return res;

}

void PreviewProvider::setPreview(const QImage& preview) {
  QMutexLocker lock(&m_mutex);

  m_image = preview;
}

PreviewProvider *PreviewProvider::instance() {
  return m_instance;
}
