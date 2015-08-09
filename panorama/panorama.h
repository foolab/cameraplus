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

#define MAX_TRACKER_FRAMES 100

#include <QObject>
#include <stdint.h>
#include <QMutex>
#include <vector>

class PanoramaInput;
class PanoramaTracker;
class PanoramaStitcher;
class QSize;

class Panorama : public QObject {
  Q_OBJECT
  Q_PROPERTY(int frameCount READ frameCount NOTIFY frameCountChanged);
  Q_PROPERTY(int stitchingProgress READ stitchingProgress NOTIFY stitchingProgressChanged);
  Q_PROPERTY(Status status READ status NOTIFY statusChanged);
  Q_PROPERTY(PanoramaInput* input READ input WRITE setInput NOTIFY inputChanged);
  Q_PROPERTY(bool keepFrames READ keepFrames WRITE setKeepFrames NOTIFY keepFramesChanged);
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

  bool keepFrames() const;
  void setKeepFrames(bool keep);

  static void clear(std::vector<uint8_t *>& v);

signals:
  void frameCountChanged();
  void stitchingProgressChanged();
  void statusChanged();
  void inputChanged();
  void keepFramesChanged();

public slots:
  void start(const QString& output);
  void stop();
  void stitch();

private slots:
  void inputDataAvailable(uint8_t *data, const QSize& size);
  void trackerFrameCountChanged();
  void stitchingDone();

private:
  QMutex m_lock;
  PanoramaInput *m_input;
  PanoramaTracker *m_tracker;
  PanoramaStitcher *m_stitcher;
  QString m_output;
  bool m_keepFrames;
};

#endif /* PANORAMA_H */
