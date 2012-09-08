// -*- c++ -*-

#ifndef EXPOSURE_H
#define EXPOSURE_H

#include "capability.h"

class QtCamExposure;

class Exposure : public Capability {
  Q_OBJECT

  Q_PROPERTY(unsigned int value READ value WRITE setValue NOTIFY valueChanged);
  Q_PROPERTY(unsigned int minimum READ minimum NOTIFY minimumChanged);
  Q_PROPERTY(unsigned int maximum READ maximum NOTIFY maximunmChanged);

public:
  Exposure(QObject *parent = 0);
  ~Exposure();

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

  QtCamExposure *m_exposure;
};

#endif /* EXPOSURE_H */
