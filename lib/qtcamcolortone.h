// -*- c++ -*-

#ifndef QT_CAM_COLOR_TONE_H
#define QT_CAM_COLOR_TONE_H

#include "qtcamcapability.h"

class QtCamColorTone : public QtCamCapability {
  Q_OBJECT

public:
  typedef enum {
    Normal = 0,
    Sepia = 1,
    Negative = 2,
    GrayScale = 3,
    Natural = 4,
    Vivid = 5,
    ColorSwap = 6,
    Solarize = 7,
    OutOfFocus = 8,
    SkyBlue = 9,
    GrassGreen = 10,
    SkinWhite = 11,
  } ColorToneMode;

  QtCamColorTone(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamColorTone();

  ColorToneMode value();
  bool setValue(const ColorToneMode& mode);
};

#endif /* QT_CAM_COLOR_TONE_H */
