// -*- c++ -*-

#ifndef WHITE_BALANCE_H
#define WHITE_BALANCE_H

#include "capability.h"
#include "qtcamwhitebalance.h"

class WhiteBalance : public Capability {
  Q_OBJECT

  Q_PROPERTY(WhiteBalanceMode value READ value WRITE setValue NOTIFY valueChanged);
  Q_ENUMS(WhiteBalanceMode);

public:
  typedef enum {
    Auto = QtCamWhiteBalance::Auto,
    Daylight = QtCamWhiteBalance::Daylight,
    Cloudy = QtCamWhiteBalance::Cloudy,
    Sunset = QtCamWhiteBalance::Sunset,
    Tungsten = QtCamWhiteBalance::Tungsten,
    Flourescent = QtCamWhiteBalance::Flourescent
  } WhiteBalanceMode;

  WhiteBalance(QObject *parent = 0);
  ~WhiteBalance();

  WhiteBalanceMode value();
  void setValue(const WhiteBalanceMode& mode);

signals:
  void valueChanged();

private:
  virtual void deviceChanged();

  QtCamWhiteBalance *m_wb;
};

#endif /* WHITE_BALANCE_H */
