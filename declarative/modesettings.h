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

#ifndef MODE_SETTINGS_H
#define MODE_SETTINGS_H

#include <QObject>
#include <QStringList>
#include <QSize>

class Camera;
class QtCamModeSettings;
class ResolutionModel;
class Resolution;
class QtCamResolution;

class ModeSettings : public QObject {
  Q_OBJECT

  Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged);
  Q_PROPERTY(QString suffix READ suffix NOTIFY settingsChanged);
  Q_PROPERTY(QStringList aspectRatios READ aspectRatios NOTIFY settingsChanged);
  Q_PROPERTY(int aspectRatioCount READ aspectRatioCount NOTIFY aspectRatioCountChanged);
  Q_PROPERTY(ResolutionModel *resolutions READ resolutions NOTIFY resolutionsChanged);
  Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged);

public:
  ModeSettings(QObject *parent = 0);
  virtual ~ModeSettings();

  QString suffix() const;
  QStringList aspectRatios() const;

  Camera *camera();
  void setCamera(Camera *camera);

  ResolutionModel *resolutions();

  bool isReady() const;

  Q_INVOKABLE bool setResolution(const QString& resolution);
  Q_INVOKABLE QString aspectRatioForResolution(const QString& resolution);
  Q_INVOKABLE QString bestResolution(const QString& aspectRatio, const QString& resolution);
  Q_INVOKABLE bool setViewfinderResolution(const QSize& resolution);

  int aspectRatioCount() const;

signals:
  void settingsChanged();
  void cameraChanged();
  void resolutionsChanged();
  void readyChanged();
  void aspectRatioCountChanged();

protected:
  virtual void resetSettings() = 0;
  virtual bool applyResolution(const QtCamResolution& resolution) = 0;

  void setSettings(QtCamModeSettings *settings);

  Camera *m_cam;

private slots:
  void deviceChanged();
  void prepareForDeviceChange();
  void resolutionsUpdated();

private:
  bool setResolution(const QtCamResolution& resolution);

  QtCamModeSettings *m_settings;
  ResolutionModel *m_resolutions;
  QSize m_pendingViewfinderResolution;
  QString m_pendingResolution;
};

#endif /* MODE_SETTINGS_H */
