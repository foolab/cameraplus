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

#ifndef VIEWFINDER_HANDLER_H
#define VIEWFINDER_HANDLER_H

#include <QObject>
#include <QMutex>
#include <QMetaMethod>

class Camera;
class QtCamDevice;

class ViewfinderHandler : public QObject {
  Q_OBJECT

  Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged);
  Q_PROPERTY(QObject* handler READ handler WRITE setHandler NOTIFY handlerChanged);
  Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged);

public:
  ViewfinderHandler(const char *slot, QObject *parent = 0);
  virtual ~ViewfinderHandler();

  Camera *camera() const;
  void setCamera(Camera *camera);

  QObject *handler() const;
  void setHandler(QObject *handler);

  bool isEnabled() const;
  void setEnabled(bool enabled);

signals:
  void cameraChanged();
  void handlerChanged();
  void enabledChanged();

private slots:
  void deviceChanged();

protected slots:
  void deviceAboutToChange();

protected:
  virtual void update();

  virtual void registerHandler(QtCamDevice *dev) = 0;
  virtual void unregisterHandler(QtCamDevice *dev) = 0;

  const QString m_slot;
  Camera *m_cam;
  QObject *m_handler;
  QtCamDevice *m_dev;
  QMutex m_mutex;
  QMetaMethod m_method;
  bool m_enabled;
};

#endif /* VIEWFINDER_HANDLER_H */
