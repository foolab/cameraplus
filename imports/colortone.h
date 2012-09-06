// -*- c++ -*-

#ifndef COLOR_TONE_H
#define COLOR_TONE_H

#include "capability.h"
#include "qtcamcolortone.h"

class ColorTone : public Capability {
  Q_OBJECT

  Q_PROPERTY(ColorToneMode value READ value WRITE setValue NOTIFY valueChanged);
  Q_ENUMS(ColorToneMode);

public:
  typedef enum {
    Normal = QtCamColorTone::Normal,
    Sepia = QtCamColorTone::Sepia,
    Negative = QtCamColorTone::Negative,
    GrayScale = QtCamColorTone::GrayScale,
    Natural = QtCamColorTone::Natural,
    Vivid = QtCamColorTone::Vivid,
    ColorSwap = QtCamColorTone::ColorSwap,
    Solarize = QtCamColorTone::Solarize,
    OutOfFocus = QtCamColorTone::OutOfFocus,
    SkyBlue = QtCamColorTone::SkyBlue,
    GrassGreen = QtCamColorTone::GrassGreen,
    SkinWhite = QtCamColorTone::SkinWhite,
  } ColorToneMode;

  ColorTone(QObject *parent = 0);
  ~ColorTone();

  ColorToneMode value();
  void setValue(const ColorToneMode& mode);

signals:
  void valueChanged();

private:
  virtual void deviceChanged();

  QtCamColorTone *m_color;
};

#endif /* COLOR_TONE_H */
