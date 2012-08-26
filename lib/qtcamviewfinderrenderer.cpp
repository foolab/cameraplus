#include "qtcamviewfinderrenderer.h"
#include "qtcamconfig.h"
#include <QMap>
#include <QDebug>

static QMap<QString, QMetaObject> _renderers;

QtCamViewfinderRenderer::QtCamViewfinderRenderer(QtCamConfig *config, QObject *parent) :
  QObject(parent) {

  Q_UNUSED(config);
}

QtCamViewfinderRenderer::~QtCamViewfinderRenderer() {

}

QtCamViewfinderRenderer *QtCamViewfinderRenderer::create(QtCamConfig *config, QObject *parent) {
  QString key = config->viewfinderRenderer();
  if (!_renderers.contains(key)) {
    qCritical() << "Unknown renderer" << key;
    return 0;
  }

  QObject *obj = _renderers[key].newInstance(Q_ARG(QtCamConfig *, config),
					     Q_ARG(QObject *, parent));

  if (!obj) {
    qCritical() << "Failed to create renderer" << key;
    return 0;
  }

  return dynamic_cast<QtCamViewfinderRenderer *>(obj);
}

int QtCamViewfinderRenderer::registerRenderer(const QString& key, const QMetaObject& meta) {
  _renderers[key] = meta;
  return _renderers.size() - 1;
}
