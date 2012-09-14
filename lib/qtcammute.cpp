#include "qtcammute.h"
#include "qtcamdevice.h"
#include "qtcamdevice_p.h"
#include <QPointer>

class QtCamMutePrivate {
public:
  static void mute_notify(GObject *gobject, GParamSpec *pspec, QtCamMute *q) {
    Q_UNUSED(gobject);
    Q_UNUSED(pspec);

    QMetaObject::invokeMethod(q, "stateChanged", Qt::QueuedConnection);
  }

  QPointer<QtCamDevice> dev;
  gulong handler;
};

QtCamMute::QtCamMute(QtCamDevice *dev, QObject *parent) :
  QObject(parent), d_ptr(new QtCamMutePrivate) {

  d_ptr->dev = dev;
  d_ptr->handler = 0;

  if (d_ptr->dev->d_ptr->cameraBin) {
    d_ptr->handler = g_signal_connect(d_ptr->dev->d_ptr->cameraBin,
				      "notify::mute",
				      G_CALLBACK(QtCamMutePrivate::mute_notify), this);
  }
}

QtCamMute::~QtCamMute() {
  if (d_ptr->dev && d_ptr->handler) {
    g_signal_handler_disconnect(d_ptr->dev->d_ptr->cameraBin, d_ptr->handler);
  }

  delete d_ptr; d_ptr = 0;
}

void QtCamMute::setEnabled(bool enabled) {
  gboolean val = enabled ? TRUE : FALSE;

  if (d_ptr->dev->d_ptr->cameraBin) {
    g_object_set(d_ptr->dev->d_ptr->cameraBin, "mute", val, NULL);
  }
}

bool QtCamMute::isEnabled() const {
  gboolean val = FALSE;

  if (d_ptr->dev->d_ptr->cameraBin) {
    g_object_get(d_ptr->dev->d_ptr->cameraBin, "mute", &val, NULL);
  }

  return (val == TRUE);
}
