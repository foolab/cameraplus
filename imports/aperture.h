// -*- c++ -*-

#ifndef APERTURE_H
#define APERTURE_H

#include "capability.h"

class QtCamAperture;

class Aperture : public Capability {
  Q_OBJECT

  Q_PROPERTY(unsigned int value READ value WRITE setValue NOTIFY valueChanged);
  Q_PROPERTY(unsigned int minimum READ minimum NOTIFY minimumChanged);
  Q_PROPERTY(unsigned int maximum READ maximum NOTIFY maximunmChanged);

public:
  Aperture(QObject *parent = 0);
  ~Aperture();

  unsigned int value();
  void setValue(unsigned int val);

  unsigned int minimum();
  unsigned int maximum();

signals:
  void valueChanged();
  void minimumChanged();
  void maximunmChanged();

private:
  virtual void deviceChanged();

  QtCamAperture *m_aperture;
};

#endif /* APERTURE_H */
