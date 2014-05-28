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

#ifndef DISPLAY_STATE_H
#define DISPLAY_STATE_H

#include <QObject>

class QTimer;
class ContextProperty;

class DisplayState : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool inhibitDim READ isDimInhibited WRITE setInhibitDim NOTIFY inhibitDimChanged);
  Q_PROPERTY(bool isOn READ isOn NOTIFY isOnChanged);

public:
  DisplayState(QObject *parent = 0);
  ~DisplayState();

  bool isDimInhibited() const;
  void setInhibitDim(bool inhibit);

  bool isOn();

signals:
  void inhibitDimChanged();
  void isOnChanged();

private slots:
  void timeout();

private:
  ContextProperty *m_state;
  QTimer *m_timer;
};

#endif /* DISPLAY_STATE_H */
