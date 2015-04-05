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

#include "qtcamresolution.h"

class QtCamResolutionPrivate : public QSharedData {
public:
  QtCamResolutionPrivate() :
    fps(-1),
    nightFps(-1),
    zslFps(-1),
    megaPixels(-1) {
  }

  QString id;
  QString aspectRatio;
  QString commonName;
  QSize capture;
  QSize preview;
  QSize viewfinder;
  int fps;
  int nightFps;
  int zslFps;
  float megaPixels;
  QtCamResolution::Mode mode;
  QVariant device;
};

QtCamResolution::QtCamResolution(const QString& id,
				 const QString& aspectRatio, const QSize& capture,
				 const QSize& preview, const QSize& viewfinder,
				 int fps, int nightFps, int zslFps, float megaPixels,
				 const QString& commonName, const Mode& mode,
				 const QVariant device) :
  d_ptr(new QtCamResolutionPrivate) {

  d_ptr->id = id;
  d_ptr->aspectRatio = aspectRatio;
  d_ptr->commonName = commonName;
  d_ptr->capture = capture;
  d_ptr->preview = preview;
  d_ptr->viewfinder = viewfinder;
  d_ptr->fps = fps;
  d_ptr->nightFps = nightFps;
  d_ptr->zslFps = zslFps;
  d_ptr->megaPixels = megaPixels;
  d_ptr->mode = mode;
  d_ptr->device = device;
}


QtCamResolution::QtCamResolution(const QtCamResolution& other) :
  d_ptr(other.d_ptr) {

}

QtCamResolution& QtCamResolution::operator=(const QtCamResolution& other) {
  d_ptr = other.d_ptr;

  return *this;
}

QtCamResolution::QtCamResolution(const QtCamResolution::Mode& mode) :
  d_ptr(new QtCamResolutionPrivate) {

  d_ptr->mode = mode;
}

QtCamResolution::~QtCamResolution() {
  // QSharedData will take care of reference counting.
}

QString QtCamResolution::id() const {
  return d_ptr->id;
}

void QtCamResolution::setId(const QString& id) {
  d_ptr->id = id;
}

QString QtCamResolution::aspectRatio() const {
  return d_ptr->aspectRatio;
}

void QtCamResolution::setAspectRatio(const QString& aspectRatio) {
  d_ptr->aspectRatio = aspectRatio;
}

QString QtCamResolution::commonName() const {
  return d_ptr->commonName;
}

void QtCamResolution::setCommonName(const QString& commonName) {
  d_ptr->commonName = commonName;
}

QSize QtCamResolution::captureResolution() const {
  return d_ptr->capture;
}

void QtCamResolution::setCaptureResolution(const QSize& captureResolution) {
  d_ptr->capture = captureResolution;
}

QSize QtCamResolution::viewfinderResolution() const {
  return d_ptr->viewfinder;
}

void QtCamResolution::setViewfinderResolution(const QSize& viewfinderResolution) {
  d_ptr->viewfinder = viewfinderResolution;
}

QSize QtCamResolution::previewResolution() const {
  return d_ptr->preview;
}

void QtCamResolution::setPreviewResolution(const QSize& previewResolution) {
  d_ptr->preview = previewResolution;
}

int QtCamResolution::frameRate() const {
  return d_ptr->fps;
}

void QtCamResolution::setFrameRate(int frameRate) {
  d_ptr->fps = frameRate;
}

int QtCamResolution::nightFrameRate() const {
  return d_ptr->nightFps;
}

void QtCamResolution::setNightFrameRate(int nightFrameRate) {
  d_ptr->nightFps = nightFrameRate;
}

int QtCamResolution::zslFrameRate() const {
  return d_ptr->zslFps;
}

void QtCamResolution::setZslFrameRate(int zslFrameRate) {
  d_ptr->zslFps = zslFrameRate;
}

float QtCamResolution::megaPixels() const {
  return d_ptr->megaPixels;
}

void QtCamResolution::setMegaPixels(float megaPixels) {
  d_ptr->megaPixels = megaPixels;
}

QtCamResolution::Mode QtCamResolution::mode() const {
  return d_ptr->mode;
}

void QtCamResolution::setMode(const QtCamResolution::Mode& mode) {
  d_ptr->mode = mode;
}

QVariant QtCamResolution::device() const {
  return d_ptr->device;
}

void QtCamResolution::setDevice(const QVariant& device) {
  d_ptr->device = device;
}

bool QtCamResolution::isValid() const {
  return d_ptr->capture.isValid() &&
    d_ptr->preview.isValid() &&
    d_ptr->viewfinder.isValid() &&
    d_ptr->fps > -1;
}

QTextStream& operator<<(QTextStream& s, const QtCamResolution& res) {
#define DUMP_RES(rr) res.d_ptr->rr.width() << "x" << res.d_ptr->rr.height()
  s << "\n[" << res.d_ptr->id << "]" << "\n"
    << "aspect ratio: " << res.d_ptr->aspectRatio << "\n"
    << "common name: " << res.d_ptr->commonName << "\n"
    << "capture: " << DUMP_RES(capture) << "\n"
    << "preview: " << DUMP_RES(preview) << "\n"
    << "viewfinder: " << DUMP_RES(viewfinder) << "\n"
    << "fps: " << res.d_ptr->fps << "\n"
    << "night fp: " << res.d_ptr->nightFps << "\n"
    << "zsl fps: " << res.d_ptr->zslFps << "\n"
    << "megapixels: " << res.d_ptr->megaPixels << "\n"
    << "device: " << res.d_ptr->device.toString() << "\n"
    << "mode: " <<  res.d_ptr->mode << "\n";

  return s;
}
