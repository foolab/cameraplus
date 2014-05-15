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

#ifndef QT_CAM_QUIRKS_H
#define QT_CAM_QUIRKS_H

#include <QObject>

class QtCamQuirksPrivate;
class QtCamDevice;

class QtCamQuirks : public QObject {
  Q_OBJECT

  Q_ENUMS(QuirkType);

public:
  typedef enum {
    NoNightSceneMode = 0x1,
    NoFlash = 0x2,
    NoVideoTorch = 0x4,
    NoAutoFocus = 0x8,
    NoRedEyeFlash = 0x20,
    NoTouchFocus = 0x40,
    NoFaceDetection = 0x80,
  } QuirkType;

  Q_DECLARE_FLAGS(QuirkTypes, QuirkType);

  QtCamQuirks(const QuirkTypes& types, QObject *parent = 0);
  ~QtCamQuirks();

  QuirkTypes quirks() const;

private:
  QtCamQuirksPrivate *d_ptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QtCamQuirks::QuirkTypes);

#endif /* QT_CAM_QUIRKS_H */
