// -*- c++ -*-

#ifndef NOISE_REDUCTION_H
#define NOISE_REDUCTION_H

#include "capability.h"
#include "qtcamnoisereduction.h"

class NoiseReduction : public Capability {
  Q_OBJECT

  Q_PROPERTY(NoiseReductionMode value READ value WRITE setValue NOTIFY valueChanged);
  Q_ENUMS(NoiseReductionMode);

public:
  typedef enum {
    None = QtCamNoiseReduction::None,
    Bayer = QtCamNoiseReduction::Bayer,
    Ycc = QtCamNoiseReduction::Ycc,
    Temporal = QtCamNoiseReduction::Temporal,
    Fixed = QtCamNoiseReduction::Fixed,
    Extra = QtCamNoiseReduction::Extra
  } NoiseReductionMode;

  NoiseReduction(QObject *parent = 0);
  ~NoiseReduction();

  NoiseReductionMode value();
  void setValue(const NoiseReductionMode& mode);

signals:
  void valueChanged();

private:
  virtual void deviceChanged();

  QtCamNoiseReduction *m_nr;
};

#endif /* NOISE_REDUCTION_H */
