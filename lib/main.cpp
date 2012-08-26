#include <QApplication>
#include <QDebug>
#include "qtcamera.h"
#include "qtcamdevice.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGLWidget>
#include "main.h"
#include "qtcamgraphicsviewfinder.h"
#include <QPushButton>
#include <QToolBar>
#include <QAction>
#include <QToolButton>
#include <QActionGroup>
#include "qtcamimagemode.h"
#include "qtcamvideomode.h"
#include <QPushButton>
#include "qtcammetadata.h"
#include <QDateTime>

Camera::Camera(QWidget *parent) :
  QWidget(parent),
  m_cam(new QtCamera("camera.ini")),
  m_dev(0), m_vf(0) {

  setMinimumSize(700, 500);

  QGraphicsView *v = new QGraphicsView;
  v->setViewport(new QGLWidget);
  v->setMinimumSize(640, 480);
  m_scene = new QGraphicsScene;
  v->setScene(m_scene);

  //  m_vf = new QtCamGraphicsViewfinder(m_cam->config());
  //  m_vf->resize(640, 480);
  //  s->addItem(m_vf);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(v);

  QToolBar *tools = new QToolBar(this);
  layout->addWidget(tools);

  QToolButton *devs = new QToolButton(this);
  devs->setArrowType(Qt::DownArrow);
  devs->setPopupMode(QToolButton::InstantPopup);
  tools->addWidget(devs);

  QActionGroup *group = new QActionGroup(devs);
  QObject::connect(group, SIGNAL(triggered(QAction *)), this, SLOT(setDevice(QAction *)));

  QList<QPair<QString, QVariant> > devices = m_cam->devices();
  for (int x = 0; x < devices.size(); x++) {
    QAction *a = group->addAction(devices[x].first);
    a->setData(devices[x].second);
    a->setCheckable(true);
    devs->addAction(a);
  }

  tools->addSeparator();
  tools->addAction(tr("Image"), this, SLOT(switchToImage()));
  tools->addAction(tr("Video"), this, SLOT(switchToVideo()));

  QList<QAction *> actions = group->actions();
  if (!actions.isEmpty()) {
    actions[0]->setChecked(true);
    setDevice(actions[0]);
  }

  m_button = new QPushButton(tr("Capture"));
  tools->addWidget(m_button);
  QObject::connect(m_button, SIGNAL(clicked()), this, SLOT(buttonClicked()));

  m_meta = new QtCamMetaData(this);
  m_meta->setDevice(m_dev);
}

Camera::~Camera() {
  if (m_dev) {
    m_dev->stop();
  }
}

void Camera::setDevice(QAction *action) {
  QVariant id = action->data();

  if (m_dev) {
    m_dev->stop();
    delete m_dev;
    delete m_vf;
  }

  m_vf = new QtCamGraphicsViewfinder(m_cam->config());
  m_vf->resize(640, 480);
  m_scene->addItem(m_vf);
  m_vf->show();

  m_dev = m_cam->device(id);
  m_dev->setViewfinder(m_vf);
  m_dev->start();
}

void Camera::switchToImage() {
  m_dev->imageMode()->activate();
}

void Camera::switchToVideo() {
  m_dev->videoMode()->activate();
}

void Camera::buttonClicked() {
  if (m_dev->activeMode() == m_dev->imageMode()) {
    m_meta->reset();
    m_meta->setArtist("Mohammed");
    m_meta->setDateTime(QDateTime::currentDateTime());
    m_dev->imageMode()->capture("/tmp/foo.jpg");
  }
  else if (m_dev->activeMode() == m_dev->videoMode()) {
    if (m_dev->videoMode()->isRecording()) {
      m_dev->videoMode()->stopRecording();
      m_button->setText(tr("Capture"));
    }
    else {
    m_meta->reset();
    m_meta->setArtist("Mohammed");
    m_meta->setDateTime(QDateTime::currentDateTime());
      m_dev->videoMode()->startRecording("/tmp/foo.ogg");
      m_button->setText(tr("Stop"));
    }
  }
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  Camera c;
  c.show();

  return app.exec();
}
