// -*- c++ -*-

#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <QObject>

namespace MeeGo {
  class QmOrientation;
  class QmOrientationReading;
};

class Orientation : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged);
  Q_PROPERTY(OrientationDirection orientation READ orientation NOTIFY orientationChanged);

  Q_ENUMS(OrientationDirection);

public:
  Orientation(QObject *parent = 0);
  ~Orientation();

  // Make sure they are synced with metadata.
  typedef enum {
    Unknown = -1,
    Landscape = 0,
    Portrait = 1,
    InvertedLandscape = 2,
    InvertedPortrait = 3
  } OrientationDirection;

  bool isActive() const;
  void setActive(bool active);

  OrientationDirection orientation() const;

signals:
  void activeChanged();
  void orientationChanged();

private slots:
  void orientationChanged(const MeeGo::QmOrientationReading& value);

private:
  MeeGo::QmOrientation *m_orientation;
  OrientationDirection m_direction;
};

#endif /* ORIENTATION_H */
