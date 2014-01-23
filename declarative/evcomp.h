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

#ifndef EV_COMP_H
#define EV_COMP_H

#include "capability.h"

class QtCamEvComp;
class QtCamDevice;

class EvComp : public Capability {
  Q_OBJECT

  Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged);
  Q_PROPERTY(qreal minimum READ minimum NOTIFY minimumChanged);
  Q_PROPERTY(qreal maximum READ maximum NOTIFY maximunmChanged);

public:
  EvComp(QtCamDevice *dev, QObject *parent = 0);
  ~EvComp();

  qreal value();
  void setValue(qreal val);

  qreal minimum();
  qreal maximum();

signals:
  void valueChanged();
  void minimumChanged();
  void maximunmChanged();

private:
  QtCamEvComp *m_evComp;
};

#endif /* EV_COMP_H */
