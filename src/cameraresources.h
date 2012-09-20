// -*- c++ -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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

#ifndef CAMERA_RESOURCES_H
#define CAMERA_RESOURCES_H

#include <QObject>
#include <policy/resource-set.h>

class CameraResources : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool acquired READ acquired NOTIFY acquiredChanged);

  Q_ENUMS(Mode);

public:
  typedef enum {
    None,
    Image,
    Video,
    Recording,
    PostCapture,
  } Mode;

  CameraResources(QObject *parent = 0);
  ~CameraResources();

  bool acquired() const;

public slots:
  void acquire(const Mode& mode);

signals:
  void acquiredChanged();

private slots:
  void resourcesReleased();
  void lostResources();
  void resourcesGranted(const QList<ResourcePolicy::ResourceType>& optional);
  void updateOK();

private:
  void updateSet(const QList<ResourcePolicy::ResourceType>& required,
		 const QList<ResourcePolicy::ResourceType>& optional =
		 QList<ResourcePolicy::ResourceType>());

  QList<ResourcePolicy::ResourceType> listSet();

  ResourcePolicy::ResourceSet *m_set;
  Mode m_mode;
  bool m_acquired;
};

#endif /* CAMERA_RESOURCES_H */
