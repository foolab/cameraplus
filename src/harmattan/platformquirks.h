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

#ifndef PLATFORM_QUIRKS_H
#define PLATFORM_QUIRKS_H

#include <QObject>

class PlatformQuirks : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool windowActive READ isWindowActive NOTIFY windowActiveChanged);

public:
  PlatformQuirks(QObject *parent = 0);
  ~PlatformQuirks();

  bool isWindowActive() const;

protected:
  bool eventFilter(QObject *obj, QEvent *event);

signals:
  void windowActiveChanged();

private:
  Qt::WindowStates state();
  bool coveredByNetworkOrTermsDialog();

  bool m_active;
};

#endif /* PLATFORM_QUIRKS_H */
