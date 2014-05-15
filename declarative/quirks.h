// -*- c++ -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2014 Mohammed Sameer <msameer@foolab.org>
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

#ifndef QUIRKS_H
#define QUIRKS_H

#include <QObject>
#include "qtcamquirks.h"

class Quirks : public QObject {
  Q_OBJECT

  Q_ENUMS(QuirkType);

public:
  Quirks(QtCamQuirks *quirks, QObject *parent = 0);
  ~Quirks();

  typedef enum {
    NoNightSceneMode = QtCamQuirks::NoNightSceneMode,
    NoFlash = QtCamQuirks::NoFlash,
    NoVideoTorch = QtCamQuirks::NoVideoTorch,
    NoAutoFocus = QtCamQuirks::NoAutoFocus,
    NoRedEyeFlash = QtCamQuirks::NoRedEyeFlash,
    NoTouchFocus = QtCamQuirks::NoTouchFocus,
    NoFaceDetection = QtCamQuirks::NoFaceDetection,
  } QuirkType;

  Q_INVOKABLE bool hasQuirk(const QuirkType& type);

private:
  QtCamQuirks *m_quirks;
};

#endif /* QUIRKS_H */
