// -*- c++ -*-

#ifndef PREVIEW_PROVIDER_H
#define PREVIEW_PROVIDER_H

#include <QDeclarativeImageProvider>
#include <QMutex>

class PreviewProvider : public QDeclarativeImageProvider {
public:
  PreviewProvider();
  ~PreviewProvider();

  static PreviewProvider *instance();

  virtual QImage requestImage(const QString& id, QSize *size, const QSize& requestedSize);
  void setPreview(const QImage& preview);

private:
  static PreviewProvider *m_instance;
  QImage m_image;
  QMutex m_mutex;
};

#endif /* PREVIEW_PROVIDER_H */
