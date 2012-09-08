// -*- c++ -*-

#ifndef ISO_H
#define ISO_H

#include "capability.h"

class QtCamIso;

class Iso : public Capability {
  Q_OBJECT

  Q_PROPERTY(unsigned int value READ value WRITE setValue NOTIFY valueChanged);
  Q_PROPERTY(unsigned int minimum READ minimum NOTIFY minimumChanged);
  Q_PROPERTY(unsigned int maximum READ maximum NOTIFY maximunmChanged);

public:
  Iso(QObject *parent = 0);
  ~Iso();

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

  QtCamIso *m_iso;
};

#endif /* ISO_H */
