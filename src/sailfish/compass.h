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

#ifndef COMPASS_H
#define COMPASS_H

#include <QObject>

class QCompass;

class Compass : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged);
  Q_PROPERTY(int direction READ direction NOTIFY directionChanged);
  Q_PROPERTY(bool directionValid READ isDirectionValid NOTIFY directionValidChanged);

public:
  Compass(QObject *parent = 0);
  ~Compass();

  bool isActive() const;
  void setActive(bool active);

  int direction() const;
  bool isDirectionValid() const;

signals:
  void activeChanged();
  void directionChanged();
  void directionValidChanged();

private slots:
  void readingChanged();

private:
  QCompass *m_compass;
  int m_degree;
  bool m_valid;
};

#endif /* COMPASS_H */
