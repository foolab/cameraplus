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

#include "videoresolution.h"

VideoResolution::VideoResolution(QObject *parent) :
  QObject(parent),
  m_fps(-1),
  m_nightFps(-1) {

}

VideoResolution::VideoResolution(const QtCamVideoResolution& resolution, QObject *parent) :
  QObject(parent),
  m_resolutionId(resolution.id()),
  m_name(resolution.name()),
  m_aspectRatio(resolution.aspectRatio()),
  m_commonName(resolution.resolution()),
  m_capture(resolution.captureResolution()),
  m_preview(resolution.previewResolution()),
  m_fps(resolution.frameRate()),
  m_nightFps(resolution.nightFrameRate()) {

}

VideoResolution::~VideoResolution() {

}

QtCamVideoResolution VideoResolution::resolution() {
  return QtCamVideoResolution(m_resolutionId, m_name, m_capture,
			      m_preview, m_fps, m_nightFps,
			      m_aspectRatio, m_commonName);
}

QString VideoResolution::resolutionId() const {
  return m_resolutionId;
}

void VideoResolution::setResolutionId(const QString& resolutionId) {
  if (m_resolutionId != resolutionId) {
    m_resolutionId = resolutionId;

    emit resolutionIdChanged();
  }
}

QString VideoResolution::name() const {
  return m_name;
}

void VideoResolution::setName(const QString& name) {
  if (m_name != name) {
    m_name = name;

    emit nameChanged();
  }
}

QString VideoResolution::aspectRatio() const {
  return m_aspectRatio;
}

void VideoResolution::setAspectRatio(const QString& aspectRatio) {
  if (m_aspectRatio != aspectRatio) {
    m_aspectRatio = aspectRatio;

    emit aspectRatioChanged();
  }
}

QSize VideoResolution::capture() const {
  return m_capture;
}

void VideoResolution::setCapture(const QSize& capture) {
  if (m_capture != capture) {
    m_capture = capture;

    emit captureChanged();
  }
}

QSize VideoResolution::preview() const {
  return m_preview;
}

void VideoResolution::setPreview(const QSize& preview) {
  if (m_preview != preview) {
    m_preview = preview;

    emit previewChanged();
  }
}

int VideoResolution::fps() const {
  return m_fps;
}

void VideoResolution::setFps(int fps) {
  if (m_fps != fps) {
    m_fps = fps;

    emit fpsChanged();
  }
}

int VideoResolution::nightFps() const {
  return m_nightFps;
}

void VideoResolution::setNightFps(int nightFps) {
  if (m_nightFps != nightFps) {
    m_nightFps = nightFps;

    emit nightFpsChanged();
  }
}

QString VideoResolution::commonName() const {
  return m_commonName;
}

void VideoResolution::setCommonName(const QString& commonName) {
  if (m_commonName != commonName) {
    m_commonName = commonName;

    emit commonNameChanged();
  }
}
