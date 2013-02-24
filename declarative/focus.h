// -*- c++ -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
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

#ifndef FOCUS_H
#define FOCUS_H

#include <QObject>
#include "qtcamfocus.h"

class QtCamDevice;

class Focus : public QObject {
  Q_OBJECT

  Q_PROPERTY(FocusMode value READ value WRITE setValue NOTIFY valueChanged);
  Q_ENUMS(FocusMode);

public:
  typedef enum {
    Auto = QtCamFocus::Auto,
    Macro = QtCamFocus::Macro,
    Portrait = QtCamFocus::Portrait,
    Infinity = QtCamFocus::Infinity,
    Hyperfocal = QtCamFocus::Hyperfocal,
    Extended = QtCamFocus::Extended,
    ContinuousNormal = QtCamFocus::ContinuousNormal,
    ContinuousExtended = QtCamFocus::ContinuousExtended,
    Sport = QtCamFocus::Sport,
  } FocusMode;

  Focus(QtCamDevice *dev, QObject *parent = 0);
  ~Focus();

  FocusMode value();
  void setValue(const FocusMode& mode);

signals:
  void valueChanged();

private:
  QtCamFocus *m_f;
};

#endif /* FOCUS_H */
