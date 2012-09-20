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

#ifndef NOISE_REDUCTION_H
#define NOISE_REDUCTION_H

#include "capability.h"
#include "qtcamnoisereduction.h"

class NoiseReduction : public Capability {
  Q_OBJECT

  Q_PROPERTY(NoiseReductionMode value READ value WRITE setValue NOTIFY valueChanged);
  Q_ENUMS(NoiseReductionMode);

public:
  typedef enum {
    None = QtCamNoiseReduction::None,
    Bayer = QtCamNoiseReduction::Bayer,
    Ycc = QtCamNoiseReduction::Ycc,
    Temporal = QtCamNoiseReduction::Temporal,
    Fixed = QtCamNoiseReduction::Fixed,
    Extra = QtCamNoiseReduction::Extra
  } NoiseReductionMode;

  NoiseReduction(QObject *parent = 0);
  ~NoiseReduction();

  NoiseReductionMode value();
  void setValue(const NoiseReductionMode& mode);

signals:
  void valueChanged();

private:
  virtual void deviceChanged();

  QtCamNoiseReduction *m_nr;
};

#endif /* NOISE_REDUCTION_H */
