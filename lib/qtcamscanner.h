// -*- c++ -*-

#ifndef QT_CAM_SCANNER_H
#define QT_CAM_SCANNER_H

#include <QObject>
#include <QPair>
#include <QVariant>

class QtCamConfig;
class QtCamScannerPrivate;

class QtCamScanner : public QObject {
  Q_OBJECT

public:
  QtCamScanner(QtCamConfig *conf, QObject *parent = 0);
  ~QtCamScanner();

  void refresh();
  QList<QPair<QString, QVariant> > devices() const;

private:
  QtCamScannerPrivate *d_ptr;
};

#endif /* QT_CAM_SCANNER_H */
