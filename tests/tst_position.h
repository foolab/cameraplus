#ifndef TST_POSITION_H
#define TST_POSITION_H

#include <QObject>

class tst_position : public QObject {
  Q_OBJECT

private slots:
  void viewfinderOrientationBack_data();
  void viewfinderOrientationBack();

  void viewfinderOrientationFront_data();
  void viewfinderOrientationFront();

  void exifBack_data();
  void exifBack();

  void exifFront_data();
  void exifFront();

  void exifNoDeviceOrientation();

  void portrait_data();
  void portrait();

  void viewfinderFlipped();

  void exifFlipped();
};

#endif /* TST_POSITION_H */
