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

#include "resolution.h"

Resolution::Resolution(const QtCamResolution& resolution, QObject *parent) :
  QObject(parent),
  m_resolution(resolution) {

}

Resolution::~Resolution() {

}

QtCamResolution Resolution::resolution() {
  return m_resolution;
}

QString Resolution::resolutionId() const {
  return m_resolution.id();
}

void Resolution::setResolutionId(const QString& resolutionId) {
  if (m_resolution.id() != resolutionId) {
    m_resolution.setId(resolutionId);
    emit resolutionIdChanged();
  }
}

QString Resolution::aspectRatio() const {
  return m_resolution.aspectRatio();
}

void Resolution::setAspectRatio(const QString& aspectRatio) {
  if (m_resolution.aspectRatio() != aspectRatio) {
    m_resolution.setAspectRatio(aspectRatio);
    emit aspectRatioChanged();
  }
}

QSize Resolution::capture() const {
  return m_resolution.captureResolution();
}

void Resolution::setCapture(const QSize& capture) {
  if (m_resolution.captureResolution() != capture) {
    m_resolution.setCaptureResolution(capture);
    emit captureChanged();
  }
}

QSize Resolution::preview() const {
  return m_resolution.previewResolution();
}

void Resolution::setPreview(const QSize& preview) {
  if (m_resolution.previewResolution() != preview) {
    m_resolution.setPreviewResolution(preview);
    emit previewChanged();
  }
}

QSize Resolution::viewfinder() const {
  return m_resolution.viewfinderResolution();
}

void Resolution::setViewfinder(const QSize& viewfinder) {
  if (m_resolution.viewfinderResolution() != viewfinder) {
    m_resolution.setViewfinderResolution(viewfinder);
    emit viewfinderChanged();
  }
}

int Resolution::fps() const {
  return m_resolution.frameRate();
}

void Resolution::setFps(int fps) {
  if (m_resolution.frameRate() != fps) {
    m_resolution.setFrameRate(fps);
    emit fpsChanged();
  }
}

int Resolution::nightFps() const {
  return m_resolution.nightFrameRate();
}

void Resolution::setNightFps(int nightFps) {
  if (m_resolution.nightFrameRate() != nightFps) {
    m_resolution.setNightFrameRate(nightFps);
    emit nightFpsChanged();
  }
}

float Resolution::megaPixels() const {
  return m_resolution.megaPixels();
}

void Resolution::setMegaPixels(float megaPixels) {
  if (!qFuzzyCompare(m_resolution.megaPixels(), megaPixels)) {
    m_resolution.setMegaPixels(megaPixels);

    emit megaPixelsChanged();
  }
}

QString Resolution::commonName() const {
  return m_resolution.commonName();
}

void Resolution::setCommonName(const QString& commonName) {
  if (m_resolution.commonName() != commonName) {
    m_resolution.setCommonName(commonName);
    emit commonNameChanged();
  }
}
