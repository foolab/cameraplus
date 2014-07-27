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

#ifndef AUTO_FOCUS_H
#define AUTO_FOCUS_H

#include <QObject>
#include "qtcamautofocus.h"

class QtCamDevice;

class AutoFocus : public QObject {
  Q_OBJECT
  Q_PROPERTY(Status status READ status NOTIFY valueChanged);
  Q_PROPERTY(Status cafStatus READ cafStatus NOTIFY cafValueChanged);
  Q_ENUMS(Status);

public:
  typedef enum {
    None = QtCamAutoFocus::None,
    Running = QtCamAutoFocus::Running,
    Fail = QtCamAutoFocus::Fail,
    Success = QtCamAutoFocus::Success,
  } Status;

  AutoFocus(QtCamDevice *dev, QObject *parent = 0);
  ~AutoFocus();

  Q_INVOKABLE bool startAutoFocus();
  Q_INVOKABLE bool stopAutoFocus();
  Q_INVOKABLE bool canFocus(int sceneMode);

  Status status();
  Status cafStatus();

signals:
  void valueChanged();
  void cafValueChanged();

private slots:
  void prepareForDeviceChange();

private:
  QtCamAutoFocus *m_af;
};

#endif /* AUTO_FOCUS_H */
