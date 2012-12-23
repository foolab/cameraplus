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

#ifndef QT_CAM_ANALYSIS_BIN_H
#define QT_CAM_ANALYSIS_BIN_H

#include <gst/gst.h>
#include <QList>

class QtCamAnalysisBinPrivate;
class QStringList;

class QtCamAnalysisBin {
public:
  ~QtCamAnalysisBin();

  static QtCamAnalysisBin *create(const QStringList& factories, const char *name);

  void setBlocked(bool blocked);
  bool isBlocked() const;

  GstElement *bin();

private:
  QtCamAnalysisBin(QtCamAnalysisBinPrivate *d);

  QtCamAnalysisBinPrivate *d_ptr;
};

#endif /* QT_CAM_ANALYSIS_BIN_H */
