// -*- c++ -*-

#ifndef IMAGE_MODE_H
#define IMAGE_MODE_H

#include "mode.h"

class QtCamImageMode;

class ImageMode : public Mode {
  Q_OBJECT

public:
  ImageMode(QObject *parent = 0);
  ~ImageMode();

  Q_INVOKABLE bool capture(const QString& fileName);

protected:
  virtual void preChangeMode();
  virtual void postChangeMode();
  virtual void changeMode();

private:
  QtCamImageMode *m_image;
};

#endif /* IMAGE_MODE_H */
