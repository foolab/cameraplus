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

#include "panoramastitcher.h"
#include "panorama.h"
#include <QTimer>
#include <QImageWriter>
#include <QImage>

#define PROGRESS_TIMER_INTERVAL 500

PanoramaStitcher::PanoramaStitcher(std::vector<uint8_t *> *frames, const QString& output,
				   QObject *parent) :
  QThread(parent),
  Stitcher(FRAME_WIDTH, FRAME_HEIGHT, frames->size()),
  m_output(output),
  m_frames(frames),
  m_running(true) {

  m_timer.setInterval(PROGRESS_TIMER_INTERVAL);
  QObject::connect(&m_timer, SIGNAL(timeout()), this, SIGNAL(progressChanged()));
  m_timer.start();
}

PanoramaStitcher::~PanoramaStitcher() {
  if (m_frames) {
    Panorama::clear(*m_frames);
    delete m_frames;
    m_frames = 0;
  }
}

void PanoramaStitcher::stop() {
  m_running = false;
  cancel();
  wait();
}

void PanoramaStitcher::run() {
  for (int x = 0; x < m_frames->size(); x++) {
    if (!m_running) {
      goto out;
    }
    // TODO: error
    addFrame(m_frames->at(x));
  }

  if (!m_running) {
    goto out;
  }

  // TODO: error
  stitch();

  if (!m_running) {
    goto out;
  }

  {
    QImageWriter wr(m_output, "JPG");
    int w, h;
    const unsigned char *im = image(w, h);

    QImage img(im, w, h, QImage::Format_RGB888);
    // TODO: error
    bool foo = wr.write(img);
  }

out:
  if (m_frames) {
    Panorama::clear(*m_frames);
    delete m_frames;
    m_frames = 0;
  }
  emit done();
}

int PanoramaStitcher::progress() {
  return Stitcher::progress();
}
