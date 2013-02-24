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

#ifndef VIDEO_SETTINGS_H
#define VIDEO_SETTINGS_H

#include <QObject>
#include <QStringList>

class Camera;
class QtCamVideoSettings;
class VideoResolutionModel;

class VideoSettings : public QObject {
  Q_OBJECT

  Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged);
  Q_PROPERTY(QString suffix READ suffix NOTIFY settingsChanged);
  Q_PROPERTY(QStringList aspectRatios READ aspectRatios NOTIFY settingsChanged);
  Q_PROPERTY(VideoResolutionModel *resolutions READ resolutions NOTIFY resolutionsChanged);
  Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged);

public:
  VideoSettings(QObject *parent = 0);
  ~VideoSettings();

  QString suffix() const;
  QStringList aspectRatios() const;

  Camera *camera();
  void setCamera(Camera *camera);

  VideoResolutionModel *resolutions();

  bool isReady() const;

  Q_INVOKABLE bool setResolution(const QString& aspectRatio, const QString& resolution);

signals:
  void settingsChanged();
  void cameraChanged();
  void resolutionsChanged();
  void readyChanged();

private slots:
  void deviceChanged();

private:
  Camera *m_cam;
  QtCamVideoSettings *m_settings;
  VideoResolutionModel *m_resolutions;
};

#endif /* VIDEO_SETTINGS_H */
