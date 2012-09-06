// -*- c++ -*-

#ifndef FLASH_H
#define FLASH_H

#include "capability.h"
#include "qtcamflash.h"

class Flash : public Capability {
  Q_OBJECT

  Q_PROPERTY(FlashMode value READ value WRITE setValue NOTIFY valueChanged);
  Q_ENUMS(FlashMode);

public:
  typedef enum {
    Auto = QtCamFlash::Auto,
    On = QtCamFlash::On,
    Off = QtCamFlash::Off,
    FillIn = QtCamFlash::FillIn,
    RedEye = QtCamFlash::RedEye
  } FlashMode;

  Flash(QObject *parent = 0);
  ~Flash();

  FlashMode value();
  void setValue(const FlashMode& mode);

signals:
  void valueChanged();

private:
  virtual void deviceChanged();

  QtCamFlash *m_flash;
};

#endif /* FLASH_H */
