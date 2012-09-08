// -*- c++ -*-

#ifndef FLICKER_REDUCTION_H
#define FLICKER_REDUCTION_H

#include "capability.h"
#include "qtcamflickerreduction.h"

class FlickerReduction : public Capability {
  Q_OBJECT

  Q_PROPERTY(FlickerReductionMode value READ value WRITE setValue NOTIFY valueChanged);
  Q_ENUMS(FlickerReductionMode);

public:
  typedef enum {
    Off = QtCamFlickerReduction::Off,
    FiftyHz = QtCamFlickerReduction::FiftyHz,
    SixtyHz = QtCamFlickerReduction::SixtyHz,
    Auto = QtCamFlickerReduction::Auto
  } FlickerReductionMode;

  FlickerReduction(QObject *parent = 0);
  ~FlickerReduction();

  FlickerReductionMode value();
  void setValue(const FlickerReductionMode& mode);

signals:
  void valueChanged();

private:
  virtual void deviceChanged();

  QtCamFlickerReduction *m_fr;
};

#endif /* FLICKER_REDUCTION_H */
