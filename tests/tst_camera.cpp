#include "tst_camera.h"
#include <QTest>
#include <QSignalSpy>
#include "camera.h"
#include "qtcamdevice.h"
#include "zoom.h"
#include "flash.h"
#include "scene.h"
#include "evcomp.h"
#include "whitebalance.h"
#include "colortone.h"
#include "iso.h"
#include "exposure.h"
#include "aperture.h"
#include "noisereduction.h"
#include "flickerreduction.h"
#include "focus.h"
#include "autofocus.h"
#include "roi.h"
#include "videomute.h"
#include "videotorch.h"

class tst_camera : public QObject {
  Q_OBJECT

public slots:
  void checkCapabilities();

private slots:
  void init();
  void cleanup();

  void reset();
  void startStop();
  void capabilities();

private:
  Camera *m_cam;
};

void tst_camera::init() {
  m_cam = new Camera(this);
}

void tst_camera::cleanup() {
  delete m_cam; m_cam = 0;
}

void tst_camera::reset() {
  QSignalSpy spy1(m_cam, SIGNAL(deviceIdChanged()));
  QCOMPARE(spy1.isValid(), true);

  QSignalSpy spy2(m_cam, SIGNAL(modeChanged()));
  QCOMPARE(spy2.isValid(), true);

  QSignalSpy spy3(m_cam, SIGNAL(deviceChanged()));
  QCOMPARE(spy3.isValid(), true);

  QCOMPARE(m_cam->mode(), Camera::UnknownMode);
  QCOMPARE(m_cam->deviceId(), QVariant());
  QCOMPARE(m_cam->reset(0, Camera::UnknownMode), false);

  QCOMPARE(m_cam->reset(0, Camera::ImageMode), true);
  QCOMPARE(spy1.count(), 1);
  QCOMPARE(spy2.count(), 1);
  QCOMPARE(spy3.count(), 1);

  QCOMPARE(m_cam->reset(0, Camera::VideoMode), true);
  QCOMPARE(spy1.count(), 1);
  QCOMPARE(spy2.count(), 2);
  QCOMPARE(spy3.count(), 1);

  QCOMPARE(m_cam->reset(1, Camera::ImageMode), true);
  QCOMPARE(spy1.count(), 2);
  QCOMPARE(spy2.count(), 3);
  QCOMPARE(spy3.count(), 2);

  QCOMPARE(m_cam->reset(1, Camera::VideoMode), true);
  QCOMPARE(spy1.count(), 2);
  QCOMPARE(spy2.count(), 4);
  QCOMPARE(spy3.count(), 2);
}

void tst_camera::startStop() {
  QCOMPARE(m_cam->isRunning(), false);
  QCOMPARE(m_cam->start(), false);
  QCOMPARE(m_cam->isRunning(), false);
  QCOMPARE(m_cam->device(), (QtCamDevice *)0x0);
  QCOMPARE(m_cam->reset(0, Camera::ImageMode), true);
  QCOMPARE(m_cam->isRunning(), false);
  m_cam->device()->setViewfinder(new TstViewfinder);
  QCOMPARE(m_cam->isRunning(), false);
  QCOMPARE(m_cam->start(), true);
  QCOMPARE(m_cam->isRunning(), true);

  QCOMPARE(m_cam->start(), true);
  QCOMPARE(m_cam->isRunning(), true);

  QCOMPARE(m_cam->stop(), true);
  QCOMPARE(m_cam->isRunning(), false);

  QCOMPARE(m_cam->stop(), true);
  QCOMPARE(m_cam->isRunning(), false);
}

void tst_camera::capabilities() {
  QCOMPARE(m_cam->reset(0, Camera::ImageMode), true);

  m_cam->device()->setViewfinder(new TstViewfinder);
  m_cam->start();

  QObject::connect(m_cam, SIGNAL(deviceIdChanged()), this, SLOT(checkCapabilities()));

  QCOMPARE(m_cam->reset(1, Camera::ImageMode), true);

  m_cam->stop();
}

void tst_camera::checkCapabilities() {
  QCOMPARE(m_cam->roi()->isEnabled(), false);
  QCOMPARE(m_cam->zoom()->value(), 1.0);
  QCOMPARE(m_cam->focus()->value(), Focus::Auto);
  QCOMPARE(m_cam->autoFocus()->status(), AutoFocus::None);
  QCOMPARE(m_cam->flickerReduction()->value(), FlickerReduction::Auto);
  QCOMPARE(m_cam->noiseReduction()->value(), NoiseReduction::None);
  QCOMPARE(m_cam->aperture()->value(), (unsigned int)0);
  QCOMPARE(m_cam->exposure()->value(), (unsigned int)0);
  QCOMPARE(m_cam->iso()->value(), (unsigned int)0);
  QCOMPARE(m_cam->colorTone()->value(), ColorTone::Normal);
  QCOMPARE(m_cam->whiteBalance()->value(), WhiteBalance::Auto);
  QCOMPARE(m_cam->evComp()->value(), 0.0);
  QCOMPARE(m_cam->scene()->value(), Scene::Auto);
  QCOMPARE(m_cam->flash()->value(), Flash::Auto);
  QCOMPARE(m_cam->flash()->isReady(), true);
  QCOMPARE(m_cam->videoMute()->isEnabled(), false);
  QCOMPARE(m_cam->videoTorch()->isOn(), false);
}

QTEST_APPLESS_MAIN(tst_camera);

#include "tst_camera.moc"
