#include "qtcamvideotorch.h"
#include "qtcamdevice.h"
#include "qtcamdevice_p.h"
#include <QPointer>

class QtCamVideoTorchPrivate {
public:
  static void torch_notify(GObject *gobject, GParamSpec *pspec, QtCamVideoTorch *q) {
    Q_UNUSED(gobject);
    Q_UNUSED(pspec);

    QMetaObject::invokeMethod(q, "stateChanged", Qt::QueuedConnection);
  }

  QPointer<QtCamDevice> dev;
  gulong handler;
};

QtCamVideoTorch::QtCamVideoTorch(QtCamDevice *dev, QObject *parent) :
  QObject(parent), d_ptr(new QtCamVideoTorchPrivate) {

  d_ptr->dev = dev;
  d_ptr->handler = 0;

  if (d_ptr->dev->d_ptr->videoSource) {
    d_ptr->handler = g_signal_connect(d_ptr->dev->d_ptr->videoSource,
				      "notify::video-torch",
				      G_CALLBACK(QtCamVideoTorchPrivate::torch_notify), this);
  }
}

QtCamVideoTorch::~QtCamVideoTorch() {
  if (d_ptr->dev && d_ptr->handler) {
    g_signal_handler_disconnect(d_ptr->dev->d_ptr->videoSource, d_ptr->handler);
  }

  delete d_ptr; d_ptr = 0;
}

void QtCamVideoTorch::setOn(bool on) {
  gboolean val = on ? TRUE : FALSE;

  if (d_ptr->dev->d_ptr->videoSource) {
    g_object_set(d_ptr->dev->d_ptr->videoSource, "video-torch", val, NULL);
  }
}

bool QtCamVideoTorch::isOn() const {
  gboolean val = FALSE;

  if (d_ptr->dev->d_ptr->videoSource) {
    g_object_get(d_ptr->dev->d_ptr->videoSource, "video-torch", &val, NULL);
  }

  return (val == TRUE);
}
