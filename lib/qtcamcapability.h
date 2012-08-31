// -*- c++ -*-

#ifndef QT_CAM_CAPABILITY_H
#define QT_CAM_CAPABILITY_H

#include <QObject>

class QtCamDevice;
class QtCamCapabilityPrivate;

class QtCamCapability : public QObject {
  Q_OBJECT
  Q_FLAGS(Capability Capabilities)

public:
  typedef enum {
    EvComp = (1 << 0),
    IsoSpeed = (1 << 1),
    WbMode = (1 << 2),
    ColourTone = (1 << 3),
    Scene = (1 << 4),
    Flash = (1 << 5),
    Zoom = (1 << 6),
    Focus = (1 << 7),
    Aperture = (1 << 8),
    Eposure = (1 << 9),
    Shake = (1 << 10),
    NoiseReduction = (1 << 11)
  } Capability;

  Q_DECLARE_FLAGS(Capabilities, Capability)

  static Capabilities capabilities(QtCamDevice *dev);

  QtCamCapability(QtCamCapabilityPrivate *d, QObject *parent = 0);
  virtual ~QtCamCapability();

  bool isSupported();

signals:
  void valueChanged();
  void minimumValueChanged();
  void maximumValueChanged();

protected:
  QtCamCapabilityPrivate *d_ptr;
};

#endif /* QT_CAM_CAPABILITY_H */
