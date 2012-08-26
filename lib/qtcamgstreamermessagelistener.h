// -*- c++ -*-

#ifndef QT_CAM_GSTREAMER_MESSAGE_LISTENER_H
#define QT_CAM_GSTREAMER_MESSAGE_LISTENER_H

#include <QObject>
#include <gst/gst.h>

class QtCamGStreamerMessageListenerPrivate;
class QtCamGStreamerMessageHandler;
class QtCamDevicePrivate;

class QtCamGStreamerMessageListener : public QObject {
  Q_OBJECT

public:
  QtCamGStreamerMessageListener(GstBus *bus, QtCamDevicePrivate *d, QObject *parent = 0);
  ~QtCamGStreamerMessageListener();

  void addHandler(QtCamGStreamerMessageHandler *handler);
  void removeHandler(QtCamGStreamerMessageHandler *handler);
  void addSyncHandler(QtCamGStreamerMessageHandler *handler);
  void removeSyncHandler(QtCamGStreamerMessageHandler *handler);

  void flushMessages();

signals:
  void error(const QString& message, int code, const QString& debug);
  void started();
  void stopped();

private:
  QtCamGStreamerMessageListenerPrivate *d_ptr;
};

#endif /* QT_CAM_GSTREAMER_MESSAGE_LISTENER_H */
