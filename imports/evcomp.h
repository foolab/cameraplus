// -*- c++ -*-

#ifndef EV_COMP_H
#define EV_COMP_H

#include "capability.h"

class QtCamEvComp;

class EvComp : public Capability {
  Q_OBJECT

  Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged);
  Q_PROPERTY(qreal minimum READ minimum NOTIFY minimumChanged);
  Q_PROPERTY(qreal maximum READ maximum NOTIFY maximunmChanged);

public:
  EvComp(QObject *parent = 0);
  ~EvComp();

  qreal value();
  void setValue(qreal val);

  qreal minimum();
  qreal maximum();

signals:
  void valueChanged();
  void minimumChanged();
  void maximunmChanged();

private:
  virtual void deviceChanged();

  QtCamEvComp *m_evComp;
};

#endif /* EV_COMP_H */
