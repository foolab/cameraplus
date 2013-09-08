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
  Q_PROPERTY(bool forceOn READ isOnForced NOTIFY forceOnChanged);

public:
  PlatformQuirks(QObject *parent = 0);
  ~PlatformQuirks();

  bool isOnForced();

protected:
  bool eventFilter(QObject *obj, QEvent *event);

signals:
  void forceOnChanged();

private:
  bool m_state;
  bool m_check;
};

#endif /* PLATFORM_QUIRKS_H */
