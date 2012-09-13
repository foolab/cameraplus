// -*- c++ -*-

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
