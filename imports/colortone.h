// -*- c++ -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef COLOR_TONE_H
#define COLOR_TONE_H

#include <QObject>
#include "qtcamcolortone.h"

class QtCamDevice;

class ColorTone : public QObject {
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

  ColorTone(QtCamDevice *dev, QObject *parent = 0);
  ~ColorTone();

  ColorToneMode value();
  void setValue(const ColorToneMode& mode);

signals:
  void valueChanged();

private:
  QtCamColorTone *m_color;
};

#endif /* COLOR_TONE_H */
