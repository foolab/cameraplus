// -*- c++ -*-

#ifndef DISPLAY_STATE_H
#define DISPLAY_STATE_H

#include <QObject>

class QTimer;
namespace MeeGo {
  class QmDisplayState;
};

class DisplayState : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool inhibitDim READ isDimInhibited WRITE setInhibitDim NOTIFY inhibitDimChanged);

public:
  DisplayState(QObject *parent = 0);
  ~DisplayState();

  bool isDimInhibited() const;
  void setInhibitDim(bool inhibit);

signals:
  void inhibitDimChanged();

private slots:
  void timeout();

private:
  MeeGo::QmDisplayState *m_state;
  QTimer *m_timer;
};

#endif /* DISPLAY_STATE_H */
