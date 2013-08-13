// -*- c++ -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
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

#include "imageresolution.h"

ImageResolution::ImageResolution(QObject *parent) :
  QObject(parent),
  m_fps(-1),
  m_nightFps(-1),
  m_megaPixels(-1) {

}

ImageResolution::ImageResolution(const QtCamImageResolution& resolution, QObject *parent) :
  QObject(parent),
  m_resolutionId(resolution.id()),
  m_name(resolution.name()),
  m_aspectRatio(resolution.aspectRatio()),
  m_capture(resolution.captureResolution()),
  m_preview(resolution.previewResolution()),
  m_viewfinder(resolution.viewfinderResolution()),
  m_fps(resolution.frameRate()),
  m_nightFps(resolution.nightFrameRate()),
  m_megaPixels(resolution.megaPixels()) {

}

ImageResolution::~ImageResolution() {

}

QtCamImageResolution ImageResolution::resolution() {
  return QtCamImageResolution(m_resolutionId, m_name, m_capture,
			      m_preview, m_viewfinder, m_fps,
			      m_nightFps, m_megaPixels, m_aspectRatio);
}

QString ImageResolution::resolutionId() const {
  return m_resolutionId;
}

void ImageResolution::setResolutionId(const QString& resolutionId) {
  if (m_resolutionId != resolutionId) {
    m_resolutionId = resolutionId;

    emit resolutionIdChanged();
  }
}

QString ImageResolution::name() const {
  return m_name;
}

void ImageResolution::setName(const QString& name) {
  if (m_name != name) {
    m_name = name;

    emit nameChanged();
  }
}

QString ImageResolution::aspectRatio() const {
  return m_aspectRatio;
}

void ImageResolution::setAspectRatio(const QString& aspectRatio) {
  if (m_aspectRatio != aspectRatio) {
    m_aspectRatio = aspectRatio;

    emit aspectRatioChanged();
  }
}

QSize ImageResolution::capture() const {
  return m_capture;
}

void ImageResolution::setCapture(const QSize& capture) {
  if (m_capture != capture) {
    m_capture = capture;

    emit captureChanged();
  }
}

QSize ImageResolution::preview() const {
  return m_preview;
}

void ImageResolution::setPreview(const QSize& preview) {
  if (m_preview != preview) {
    m_preview = preview;

    emit previewChanged();
  }
}

QSize ImageResolution::viewfinder() const {
  return m_viewfinder;
}

void ImageResolution::setViewfinder(const QSize& viewfinder) {
  if (m_viewfinder != viewfinder) {
    m_viewfinder = viewfinder;

    emit viewfinderChanged();
  }
}

int ImageResolution::fps() const {
  return m_fps;
}

void ImageResolution::setFps(int fps) {
  if (m_fps != fps) {
    m_fps = fps;

    emit fpsChanged();
  }
}

int ImageResolution::nightFps() const {
  return m_nightFps;
}

void ImageResolution::setNightFps(int nightFps) {
  if (m_nightFps != nightFps) {
    m_nightFps = nightFps;

    emit nightFpsChanged();
  }
}

int ImageResolution::megaPixels() const {
  return m_megaPixels;
}

void ImageResolution::setMegaPixels(int megaPixels) {
  if (m_megaPixels != megaPixels) {
    m_megaPixels = megaPixels;

    emit megaPixelsChanged();
  }
}
