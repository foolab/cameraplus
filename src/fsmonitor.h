// -*- c++ -*-

#ifndef FS_MONITOR_H
#define FS_MONITOR_H

#include <QObject>

namespace MeeGo {
  class QmUSBMode;
};

class FSMonitor : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool available READ isAvailable NOTIFY availabilityChanged)

public:
  FSMonitor(QObject *parent = 0);
  ~FSMonitor();

  bool isAvailable() const;

  void setAvailable(bool available);

signals:
  void availabilityChanged();

private slots:
  void modeChanged();

private:
  bool m_available;
  MeeGo::QmUSBMode *m_mode;
};

#endif /* FS_MONITOR_H */
