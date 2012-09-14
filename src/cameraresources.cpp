#include "cameraresources.h"
#include <QDebug>

using namespace ResourcePolicy;

CameraResources::CameraResources(QObject *parent) :
  QObject(parent),
  m_set(new ResourceSet("camera", this, true, true)),
  m_mode(None), m_acquired(false) {

  QObject::connect(m_set, SIGNAL(resourcesReleased()), this, SLOT(resourcesReleased()));
  QObject::connect(m_set, SIGNAL(lostResources()), this, SLOT(lostResources()));
  QObject::connect(m_set, SIGNAL(resourcesGranted(const QList<ResourcePolicy::ResourceType>&)),
		   this, SLOT(resourcesGranted(const QList<ResourcePolicy::ResourceType>&)));
  QObject::connect(m_set, SIGNAL(updateOK()), this, SLOT(updateOK()));

  if (!m_set->initAndConnect()) {
    qCritical() << "Failed to connect to resource policy engine";
  }
}

CameraResources::~CameraResources() {
  acquire(None);
}

void CameraResources::acquire(const CameraResources::Mode& mode) {
  if (mode == m_mode) {
    // We need to emit this because the UI migh be waiting
    emit acquiredChanged();
    return;
  }

  m_mode = mode;

  switch (m_mode) {
  case None:
    m_set->release();
    break;

  case Image:
    updateSet(QList<ResourcePolicy::ResourceType>()
	      << ResourcePolicy::VideoPlaybackType
	      << ResourcePolicy::VideoRecorderType
	      << ResourcePolicy::ScaleButtonType
	      << ResourcePolicy::SnapButtonType);
    break;

  case Video:
    updateSet(QList<ResourcePolicy::ResourceType>()
	      << ResourcePolicy::VideoPlaybackType
	      << ResourcePolicy::VideoRecorderType
	      << ResourcePolicy::ScaleButtonType
	      << ResourcePolicy::SnapButtonType);
    break;

  case Recording:
    updateSet(QList<ResourcePolicy::ResourceType>()
	      << ResourcePolicy::VideoPlaybackType
	      << ResourcePolicy::VideoRecorderType
	      << ResourcePolicy::ScaleButtonType
	      << ResourcePolicy::SnapButtonType
	      << ResourcePolicy::AudioRecorderType,
	      QList<ResourcePolicy::ResourceType>()
	      << ResourcePolicy::AudioPlaybackType);
    break;

  case PostCapture:
    updateSet(QList<ResourcePolicy::ResourceType>()
	      << ResourcePolicy::VideoPlaybackType
	      << ResourcePolicy::ScaleButtonType,
	      QList<ResourcePolicy::ResourceType>()
	      << ResourcePolicy::AudioPlaybackType);

    break;

  default:
    qWarning() << "Unknown mode" << mode;

    break;
  }
}

bool CameraResources::acquired() const {
  return m_acquired;
}

void CameraResources::resourcesReleased() {
  m_mode = None;
  m_acquired = false;
  emit acquiredChanged();
}

void CameraResources::lostResources() {
  m_mode = None;
  m_acquired = false;
  emit acquiredChanged();
}

void CameraResources::resourcesGranted(const QList<ResourcePolicy::ResourceType>& optional) {
  Q_UNUSED(optional);

  m_acquired = true;
  emit acquiredChanged();
}

void CameraResources::updateOK() {
  m_acquired = true;
  emit acquiredChanged();
}

QList<ResourcePolicy::ResourceType> CameraResources::listSet() {
  QList<Resource *> resources = m_set->resources();
  QList<ResourcePolicy::ResourceType> set;

  foreach (Resource *r, resources) {
    set << r->type();
  }

  return set;
}

void CameraResources::updateSet(const QList<ResourcePolicy::ResourceType>& required,
				const QList<ResourcePolicy::ResourceType>& optional) {

  bool isEmpty = m_set->resources().isEmpty();

  QList<ResourcePolicy::ResourceType> set = listSet();

  foreach (ResourceType r, set) {
    // Check for acquired resources that should be dropped.
    if (required.indexOf(r) == -1) {
      m_set->deleteResource(r);
    }
  }

  foreach (ResourceType r, required) {
    // TODO: AudioPlayback needs special handling
    m_set->addResource(r);
  }

  // TODO: optional resources

  // Odd. If we don't do it that way then policy ignores our requests
  // when we get minimized then maximized.
  if (isEmpty) {
    m_set->update();
  }
  else {
    m_set->acquire();
  }
}
