// -*- c++ -*-

#ifndef COMPASS_H
#define COMPASS_H

#include <QObject>

namespace MeeGo {
  class QmCompass;
  class QmCompassReading;
};

class Compass : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged);
  Q_PROPERTY(int direction READ direction NOTIFY directionChanged);
  Q_PROPERTY(bool directionValid READ isDirectionValid NOTIFY directionValidChanged);

public:
  Compass(QObject *parent = 0);
  ~Compass();

  bool isActive() const;
  void setActive(bool active);

  int direction() const;
  bool isDirectionValid() const;

signals:
  void activeChanged();
  void directionChanged();
  void directionValidChanged();

private slots:
  void dataAvailable(const MeeGo::QmCompassReading& value);

private:
  MeeGo::QmCompass *m_compass;
  int m_degree;
  bool m_valid;
};

#endif /* COMPASS_H */
