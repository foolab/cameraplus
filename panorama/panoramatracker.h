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

#ifndef PANORAMA_TRACKER_H
#define PANORAMA_TRACKER_H

#include <QThread>
#include <QMutex>
#include <vector>
#include "stitcher/tracker.h"
#include <QSize>
#include <glib.h>
#include "panorama.h"

class PanoramaInput;

class PanoramaTracker : public QThread, private Tracker {
  Q_OBJECT

public:
  PanoramaTracker(PanoramaInput *input, QObject *parent = 0);
  ~PanoramaTracker();

  int frameCount();

  void stop();
  bool isRunning();

  void releaseFrames(std::vector<guint8 *>& frames);

  QSize size();

protected:
  void run();

signals:
  void frameCountChanged();
  void error(const Panorama::Error& errorCode);

private:
  int m_width;
  int m_height;
  QSize m_inputSize;
  bool m_running;
  QMutex m_lock;
  PanoramaInput *m_input;
  std::vector<guint8 *> m_scaled;
  std::vector<guint8 *> m_frames;
};

#endif /* PANORAMA_TRACKER_H */
