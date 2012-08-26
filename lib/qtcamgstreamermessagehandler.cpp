#include "qtcamgstreamermessagehandler.h"

class QtCamGStreamerMessageHandlerPrivate {
public:
  QString name;
};

QtCamGStreamerMessageHandler::QtCamGStreamerMessageHandler(const QString& messageName,
							   QObject *parent) :
  QObject(parent), d_ptr(new QtCamGStreamerMessageHandlerPrivate) {

  d_ptr->name = messageName;
}

QtCamGStreamerMessageHandler::~QtCamGStreamerMessageHandler() {
  delete d_ptr; d_ptr = 0;
}

QString QtCamGStreamerMessageHandler::messageName() const {
  return d_ptr->name;
}

void QtCamGStreamerMessageHandler::handleMessage(GstMessage *message) {
  emit messageSent(message);
}

