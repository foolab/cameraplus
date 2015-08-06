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

#ifndef PANORAMA_H
#define PANORAMA_H

#define FRAME_WIDTH    640
#define FRAME_HEIGHT   480
#define TRACKER_WIDTH  160
#define TRACKER_HEIGHT 120
#define FRAME_Y(x) x
#define FRAME_U(x) &x[FRAME_WIDTH*FRAME_HEIGHT]
#define FRAME_V(x) &x[FRAME_WIDTH*FRAME_HEIGHT+FRAME_WIDTH/2*FRAME_HEIGHT/2]
#define TRACKER_Y(x) x
#define TRACKER_U(x) &x[TRACKER_WIDTH*TRACKER_HEIGHT]
#define TRACKER_V(x) &x[TRACKER_WIDTH*TRACKER_HEIGHT+TRACKER_WIDTH/2*TRACKER_HEIGHT/2]
#define MAX_TRACKER_FRAMES 100

#include <QObject>
#include <stdint.h>
#include <QMutex>
#include <vector>

class PanoramaInput;
class PanoramaTracker;
class PanoramaStitcher;

class Panorama : public QObject {
  Q_OBJECT
  Q_PROPERTY(int frameCount READ frameCount NOTIFY frameCountChanged);
  Q_PROPERTY(int stitchingProgress READ stitchingProgress NOTIFY stitchingProgressChanged);
  Q_PROPERTY(Status status READ status NOTIFY statusChanged);
  Q_PROPERTY(PanoramaInput* input READ input WRITE setInput NOTIFY inputChanged);
  Q_ENUMS(Status);

public:
  typedef enum {
    Idle,
    Tracking,
    Stitching,
  } Status;

  Panorama(QObject *parent = 0);
  ~Panorama();

  int frameCount();
  int stitchingProgress();
  Status status();

  PanoramaInput *input() const;
  void setInput(PanoramaInput *input);

  static void clear(std::vector<uint8_t *>& v);

signals:
  void frameCountChanged();
  void stitchingProgressChanged();
  void statusChanged();
  void inputChanged();

public slots:
  void start();
  void stop();
  void stitch();

private slots:
  void inputDataAvailable(uint8_t *data);
  void trackerFrameCountChanged();
  void stitchingDone();

private:
  QMutex m_lock;
  PanoramaInput *m_input;
  PanoramaTracker *m_tracker;
  PanoramaStitcher *m_stitcher;
};

#endif /* PANORAMA_H */
