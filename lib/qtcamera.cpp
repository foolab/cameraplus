#include "qtcamera.h"
#include "qtcamscanner.h"
#include "qtcamconfig.h"
#include "qtcamdevice.h"
#include <gst/gst.h>

class QtCameraPrivate {
public:
  QtCamConfig *conf;
  QtCamScanner *scanner;
};

QtCamera::QtCamera(QObject *parent) :
  QObject(parent), d_ptr(new QtCameraPrivate) {

  gst_init(0, 0);

  d_ptr->conf = new QtCamConfig(this);
  d_ptr->scanner = new QtCamScanner(d_ptr->conf, this);

  refreshDevices();
}

QtCamera::QtCamera(const QString& configPath, QObject *parent) :
  QObject(parent), d_ptr(new QtCameraPrivate) {

  gst_init(0, 0);

  d_ptr->conf = new QtCamConfig(configPath, this);
  d_ptr->scanner = new QtCamScanner(d_ptr->conf, this);

  refreshDevices();
}

QtCamera::QtCamera(QtCamConfig *config, QObject *parent) :
  QObject(parent), d_ptr(new QtCameraPrivate) {

  gst_init(0, 0);

  d_ptr->conf = config;
  d_ptr->scanner = new QtCamScanner(d_ptr->conf, this);

  refreshDevices();
}

QtCamera::~QtCamera() {
  delete d_ptr; d_ptr = 0;

  gst_deinit();
}

void QtCamera::refreshDevices() {
  d_ptr->scanner->refresh();
}

QList<QPair<QString, QVariant> > QtCamera::devices() const {
  return d_ptr->scanner->devices();
}

QtCamDevice *QtCamera::device(const QVariant& id, QObject *parent) {
  QList<QPair<QString, QVariant> > devs = devices();

  // G++ barfs with foreach and class templates.
  typedef QPair<QString, QVariant> Dev;
  foreach (const Dev& dev, devs) {
    if (dev.second == id) {
      return new QtCamDevice(d_ptr->conf, dev.first, dev.second, parent ? parent : this);
    }
  }

  return 0;
}

QtCamConfig *QtCamera::config() const {
  return d_ptr->conf;
}
