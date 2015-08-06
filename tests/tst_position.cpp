#include <QTest>
#include "position.h"
#include "tst_position.h"

void tst_position::viewfinderOrientationBack_data() {
  // depends on sensor orientation, application orientation and front or back camera
  QTest::addColumn<int>("sensorOrientation");
  QTest::addColumn<int>("applicationOrientation");
  QTest::addColumn<int>("viewfinderOrientation");

  QTest::newRow("0 0") << 0 << 0 << 0;
  QTest::newRow("0 90") << 0 << 90 << 270;
  QTest::newRow("0 180") << 0 << 180 << 180;
  QTest::newRow("0 270") << 0 << 270 << 90;
  QTest::newRow("90 0") << 90 << 0 << 90;
  QTest::newRow("90 90") << 90 << 90 << 0;
  QTest::newRow("90 180") << 90 << 180 << 270;
  QTest::newRow("90 270") << 90 << 270 << 180;
  QTest::newRow("180 0") << 180 << 0 << 180;
  QTest::newRow("180 90") << 180 << 90 << 90;
  QTest::newRow("180 180") << 180 << 180 << 0;
  QTest::newRow("180 270") << 180 << 270 << 270;
  QTest::newRow("270 0") << 270 << 0 << 270;
  QTest::newRow("270 90") << 270 << 90 << 180;
  QTest::newRow("270 180") << 270 << 180 << 90;
  QTest::newRow("270 270") << 270 << 270 << 0;
}

void tst_position::viewfinderOrientationBack() {
  QFETCH(int, sensorOrientation);
  QFETCH(int, applicationOrientation);
  QFETCH(int, viewfinderOrientation);

  Position pos;
  pos.setFrontCamera(false);
  pos.setSensorOrientationAngle(sensorOrientation);
  pos.setApplicationOrientationAngle(applicationOrientation);

  QCOMPARE(pos.viewfinderRotationAngle(), viewfinderOrientation);
  QCOMPARE(pos.isViewfinderFlipped(), false);
}

void tst_position::viewfinderOrientationFront_data() {
  // depends on sensor orientation, application orientation and front or back camera
  QTest::addColumn<int>("sensorOrientation");
  QTest::addColumn<int>("applicationOrientation");
  QTest::addColumn<int>("viewfinderOrientation");

  QTest::newRow("0 0") << 0 << 0 << 0;
  QTest::newRow("0 90") << 0 << 90 << 270;
  QTest::newRow("0 180") << 0 << 180 << 180;
  QTest::newRow("0 270") << 0 << 270 << 90;
  QTest::newRow("90 0") << 90 << 0 << 270;
  QTest::newRow("90 90") << 90 << 90 << 180;
  QTest::newRow("90 180") << 90 << 180 << 90;
  QTest::newRow("90 270") << 90 << 270 << 0;
  QTest::newRow("180 0") << 180 << 0 << 180;
  QTest::newRow("180 90") << 180 << 90 << 90;
  QTest::newRow("180 180") << 180 << 180 << 0;
  QTest::newRow("180 270") << 180 << 270 << 270;
  QTest::newRow("270 0") << 270 << 0 << 90;
  QTest::newRow("270 90") << 270 << 90 << 0;
  QTest::newRow("270 180") << 270 << 180 << 270;
  QTest::newRow("270 270") << 270 << 270 << 180;
}

void tst_position::viewfinderOrientationFront() {
  QFETCH(int, sensorOrientation);
  QFETCH(int, applicationOrientation);
  QFETCH(int, viewfinderOrientation);

  Position pos;
  pos.setFrontCamera(true);
  pos.setSensorOrientationAngle(sensorOrientation);
  pos.setApplicationOrientationAngle(applicationOrientation);

  QCOMPARE(pos.viewfinderRotationAngle(), viewfinderOrientation);
  QCOMPARE(pos.isViewfinderFlipped(), true);
}

void tst_position::exifBack_data() {
  // depends on sensor orientation, device orientation and front or back camera
  QTest::addColumn<int>("sensorOrientation");
  QTest::addColumn<int>("deviceOrientation");
  QTest::addColumn<int>("exifOrientation");

  QTest::newRow("0 0") << 0 << 0 << 0;
  QTest::newRow("0 90") << 0 << 90 << 270;
  QTest::newRow("0 180") << 0 << 180 << 180;
  QTest::newRow("0 270") << 0 << 270 << 90;
  QTest::newRow("90 0") << 90 << 0 << 90;
  QTest::newRow("90 90") << 90 << 90 << 0;
  QTest::newRow("90 180") << 90 << 180 << 270;
  QTest::newRow("90 270") << 90 << 270 << 180;
  QTest::newRow("180 0") << 180 << 0 << 180;
  QTest::newRow("180 90") << 180 << 90 << 90;
  QTest::newRow("180 180") << 180 << 180 << 0;
  QTest::newRow("180 270") << 180 << 270 << 270;
  QTest::newRow("270 0") << 270 << 0 << 270;
  QTest::newRow("270 90") << 270 << 90 << 180;
  QTest::newRow("270 180") << 270 << 180 << 90;
  QTest::newRow("270 270") << 270 << 270 << 0;
}

void tst_position::exifBack() {
  QFETCH(int, sensorOrientation);
  QFETCH(int, deviceOrientation);
  QFETCH(int, exifOrientation);

  Position pos;
  pos.setFrontCamera(false);
  pos.setSensorOrientationAngle(sensorOrientation);
  pos.setDeviceOrientationAngle(deviceOrientation);

  QCOMPARE(pos.exifRotationAngle(), exifOrientation);
  QCOMPARE(pos.isExifFlipped(), false);
}

void tst_position::exifFront_data() {
  // depends on sensor orientation, device orientation and front or back camera
  QTest::addColumn<int>("sensorOrientation");
  QTest::addColumn<int>("deviceOrientation");
  QTest::addColumn<int>("exifOrientation");

  QTest::newRow("0 0") << 0 << 0 << 0;
  QTest::newRow("0 90") << 0 << 90 << 90;
  QTest::newRow("0 180") << 0 << 180 << 180;
  QTest::newRow("0 270") << 0 << 270 << 270;
  QTest::newRow("90 0") << 90 << 0 << 90;
  QTest::newRow("90 90") << 90 << 90 << 180;
  QTest::newRow("90 180") << 90 << 180 << 270;
  QTest::newRow("90 270") << 90 << 270 << 0;
  QTest::newRow("180 0") << 180 << 0 << 180;
  QTest::newRow("180 90") << 180 << 90 << 270;
  QTest::newRow("180 180") << 180 << 180 << 0;
  QTest::newRow("180 270") << 180 << 270 << 90;
  QTest::newRow("270 0") << 270 << 0 << 270;
  QTest::newRow("270 90") << 270 << 90 << 0;
  QTest::newRow("270 180") << 270 << 180 << 90;
  QTest::newRow("270 270") << 270 << 270 << 180;
}

void tst_position::exifFront() {
  QFETCH(int, sensorOrientation);
  QFETCH(int, deviceOrientation);
  QFETCH(int, exifOrientation);

  Position pos;
  pos.setFrontCamera(true);
  pos.setSensorOrientationAngle(sensorOrientation);
  pos.setDeviceOrientationAngle(deviceOrientation);

  QCOMPARE(pos.exifRotationAngle(), exifOrientation);
  QCOMPARE(pos.isExifFlipped(), true);
}

void tst_position::exifNoDeviceOrientation() {
  Position pos;
  pos.setFrontCamera(false);
  pos.setSensorOrientationAngle(0);
  pos.setDeviceOrientationAngle(-1);

  QCOMPARE(pos.exifRotationAngle(), -1);
  QCOMPARE(pos.isExifFlipped(), false);

  pos.setFrontCamera(true);
  QCOMPARE(pos.exifRotationAngle(), -1);
  QCOMPARE(pos.isExifFlipped(), true);
}

void tst_position::portrait_data() {
  // depends on natural orientation and device orientation
  QTest::addColumn<int>("deviceOrientation");
  QTest::addColumn<int>("naturalOrientation");
  QTest::addColumn<bool>("portrait");

  QTest::newRow("-1 portrait") << -1 << (int)Position::Portrait << true;

  QTest::newRow("0 portrait") << 0 << (int)Position::Portrait << true;
  QTest::newRow("90 portrait") << 90 << (int)Position::Portrait << false;
  QTest::newRow("180 portrait") << 180 << (int)Position::Portrait << true;
  QTest::newRow("270 portrait") << 270 << (int)Position::Portrait << false;

  QTest::newRow("-1 landscape") << -1 << (int)Position::Landscape << false;

  QTest::newRow("0 landscape") << 0 << (int)Position::Landscape << false;
  QTest::newRow("90 landscape") << 90 << (int)Position::Landscape << true;
  QTest::newRow("180 landscape") << 180 << (int)Position::Landscape << false;
  QTest::newRow("270 landscape") << 270 << (int)Position::Landscape << true;
}

void tst_position::portrait() {
  QFETCH(int, deviceOrientation);
  QFETCH(int, naturalOrientation);
  QFETCH(bool, portrait);

  Position pos;
  pos.setNaturalOrientation((Position::NaturalOrientation)naturalOrientation);
  pos.setDeviceOrientationAngle(deviceOrientation);

  QCOMPARE(portrait, pos.isPortrait());
}

void tst_position::viewfinderFlipped() {
  Position pos;

  int orientations[5] = {-1, 0, 90, 180, 270};
  Position::NaturalOrientation naturals[2] = {Position::Portrait, Position::Landscape};

  for (int x = 0; x < 5; x++) {
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
	for (int n = 0; n < 2; n++) {
	  pos.setDeviceOrientationAngle(orientations[x]);
	  pos.setSensorOrientationAngle(orientations[i]);
	  pos.setApplicationOrientationAngle(orientations[j]);
	  pos.setNaturalOrientation(naturals[n]);

	  pos.setFrontCamera(true);
	  QCOMPARE(pos.isViewfinderFlipped(), true);

	  pos.setFrontCamera(false);
	  QCOMPARE(pos.isViewfinderFlipped(), false);
	}
      }
    }
  }
}

void tst_position::exifFlipped() {
  Position pos;

  int orientations[5] = {-1, 0, 90, 180, 270};
  Position::NaturalOrientation naturals[2] = {Position::Portrait, Position::Landscape};

  for (int x = 0; x < 5; x++) {
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
	for (int n = 0; n < 2; n++) {
	  pos.setDeviceOrientationAngle(orientations[x]);
	  pos.setSensorOrientationAngle(orientations[i]);
	  pos.setApplicationOrientationAngle(orientations[j]);
	  pos.setNaturalOrientation(naturals[n]);

	  pos.setFrontCamera(true);
	  QCOMPARE(pos.isExifFlipped(), true);

	  pos.setFrontCamera(false);
	  QCOMPARE(pos.isExifFlipped(), false);
	}
      }
    }
  }
}

QTEST_APPLESS_MAIN(tst_position);
