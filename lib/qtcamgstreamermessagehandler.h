// -*- c++ -*-

#ifndef QT_CAM_GSTREAMER_MESSAGE_HANDLER_H
#define QT_CAM_GSTREAMER_MESSAGE_HANDLER_H

#include <QObject>
#include <gst/gst.h>

class QtCamGStreamerMessageHandlerPrivate;

class QtCamGStreamerMessageHandler : public QObject {
  Q_OBJECT

public:
  QtCamGStreamerMessageHandler(const QString& messageName, QObject *parent = 0);
  virtual ~QtCamGStreamerMessageHandler();

  QString messageName() const;

  virtual void handleMessage(GstMessage *message);

signals:
  void messageSent(GstMessage *message);

private:
  QtCamGStreamerMessageHandlerPrivate *d_ptr;
};

#endif /* QT_CAM_GSTREAMER_MESSAGE_HANDLER_H */
