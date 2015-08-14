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

#ifndef QT_CAM_MODE_P_H
#define QT_CAM_MODE_P_H

#include <QSize>
#include <QFileInfo>
#include <QDir>
#include "qtcamdevice_p.h"
#include "qtcamgstmessagehandler.h"

class DoneHandler;
class PreviewImageHandler;

class QtCamModePrivate {
public:
  QtCamModePrivate(QtCamDevicePrivate *d);
  virtual ~QtCamModePrivate();

  void init(DoneHandler *handler);
  int modeId(const char *mode);
  GstEncodingProfile *loadProfile(const QString& path, const QString& name);
  void resetCaps(const char *property);
  bool inNightMode();
  void setCaps(const char *property, const QSize& resolution, int fps);
  void setPreviewSize(const QSize& size);
  void setFileName(const QString& file);
  void setTempFileName(const QString& file);
  void enableViewfinderFilters();
  void disableViewfinderFilters();

  int id;
  QtCamMode *q_ptr;
  QtCamDevicePrivate *dev;
  PreviewImageHandler *previewImageHandler;
  DoneHandler *doneHandler;
  QString fileName;
  QString tempFileName;
};

class DoneHandler : public QtCamGstMessageHandler {
public:
  DoneHandler(QtCamModePrivate *m, const char *done, QObject *parent = 0) :
    QtCamGstMessageHandler(done, parent) {
    mode = m;
  }

  virtual ~DoneHandler() { }

  virtual void handleMessage(GstMessage *message) {
    // If we have a temp file then we rename it:
    if (!mode->tempFileName.isEmpty() && !mode->fileName.isEmpty()) {
      if (!QFile::rename(mode->tempFileName, mode->fileName)) {
	qCritical() << "Failed to rename" << mode->tempFileName << "to" << mode->fileName;
      }
    }

    QString fileName;
    const GstStructure *s = gst_message_get_structure(message);
    if (gst_structure_has_field(s, "filename")) {
      const char *str = gst_structure_get_string(s, "filename");
      if (str) {
	fileName = QString::fromUtf8(str);
      }
    }

    if (fileName.isEmpty()) {
      fileName = mode->fileName;
    }

    QMetaObject::invokeMethod(mode->q_ptr, "saved", Qt::QueuedConnection,
			      Q_ARG(QString, fileName));
  }

  QtCamModePrivate *mode;
};

#endif /* QT_CAM_MODE_P_H */
