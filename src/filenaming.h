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

#ifndef FILE_NAMING_H
#define FILE_NAMING_H

#include <QObject>
#if defined(QT4)
#include <QDeclarativeParserStatus>
#elif defined(QT5)
#include <QQmlParserStatus>
#endif
#include "fileindex.h"

class Settings;
class PlatformSettings;

#if defined(QT4)
class FileNaming : public QObject, public QDeclarativeParserStatus {
  Q_INTERFACES(QDeclarativeParserStatus)
#elif defined(QT5)
class FileNaming : public QObject, public QQmlParserStatus {
#endif

  Q_OBJECT

  Q_PROPERTY(QString imageSuffix READ imageSuffix WRITE setImageSuffix NOTIFY imageSuffixChanged);
  Q_PROPERTY(QString videoSuffix READ videoSuffix WRITE setVideoSuffix NOTIFY videoSuffixChanged);
  Q_PROPERTY(Settings *settings READ settings WRITE setSettings NOTIFY settingsChanged);
  Q_PROPERTY(PlatformSettings *platformSettings READ platformSettings WRITE setPlatformSettings NOTIFY platformSettingsChanged);

public:
  FileNaming(QObject *parent = 0);
  ~FileNaming();

  QString imageSuffix() const;
  void setImageSuffix(const QString& suffix);

  QString videoSuffix() const;
  void setVideoSuffix(const QString& suffix);

  Q_INVOKABLE QString imageFileNameWithExtension(const QString& ext);
  Q_INVOKABLE QString imageFileName();
  Q_INVOKABLE QString videoFileName();
  Q_INVOKABLE QString temporaryVideoFileName();

  Settings *settings() const;
  void setSettings(Settings *settings);

  PlatformSettings *platformSettings() const;
  void setPlatformSettings(PlatformSettings *settings);

  virtual void classBegin();
  virtual void componentComplete();

signals:
  void imageSuffixChanged();
  void videoSuffixChanged();
  void settingsChanged();
  void platformSettingsChanged();

private:
  typedef enum {
    Image = FileIndex::Image,
    Video = FileIndex::Video,
  } Type;

  QString fileName(const QString& path, const QString& suffix, const Type& type);
  QString temporaryPath();

  QString m_imageSuffix;
  QString m_videoSuffix;

  Settings *m_settings;
  PlatformSettings *m_platformSettings;
  FileIndex *m_index;
};

#endif /* FILE_NAMING_H */
