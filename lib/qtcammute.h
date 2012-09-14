// -*- c++ -*-

#ifndef QT_CAM_MUTE_H
#define QT_CAM_MUTE_H

#include <QObject>

class QtCamDevice;
class QtCamMutePrivate;

class QtCamMute : public QObject {
  Q_OBJECT

public:
  QtCamMute(QtCamDevice *dev, QObject *parent = 0);
  ~QtCamMute();

  void setEnabled(bool enabled);
  bool isEnabled() const;

signals:
  void stateChanged();

private:
  QtCamMutePrivate *d_ptr;
};

#endif /* QT_CAM_MUTE_H */
