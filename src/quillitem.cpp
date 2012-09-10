#include "quillitem.h"
#include <QuillFile>
#include <QUrl>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDir>

QuillItem::QuillItem(QDeclarativeItem *parent) :
  QDeclarativeItem(parent), m_file(0) {

  setFlag(QGraphicsItem::ItemHasNoContents, false);

  static bool init = false;
  if (!init) {
    Quill::setPreviewLevelCount(1);
    Quill::setPreviewSize(0, QSize(864, 480)); // TODO:
    Quill::setMinimumPreviewSize(0, QSize(864, 480)); // TODO:
    Quill::setThumbnailExtension("jpeg"); // TODO:
    Quill::setThumbnailFlavorName(0, "screen");
    Quill::setBackgroundRenderingColor(Qt::black);
    QString tempPath(QDir::homePath() +  QDir::separator() + ".config" +
                     QDir::separator() + "quill" + QDir::separator() + "tmp");
    QDir().mkpath(tempPath);
    Quill::setTemporaryFilePath(tempPath);
    init = true;
  }
}

QuillItem::~QuillItem() {
  delete m_file; m_file = 0;
}

QUrl QuillItem::source() const {
  if (m_file) {
    return QUrl::fromLocalFile(m_file->fileName());
  }

  return QUrl();
}

void QuillItem::setSource(const QUrl& src) {
  if (src == source()) {
    return;
  }

  if (m_file) {
    m_file->deleteLater();
  }

  m_file = new QuillFile(src.toLocalFile());

  QObject::connect(m_file, SIGNAL(error(QuillError)),
	  this, SLOT(fileError()), Qt::QueuedConnection);
  QObject::connect(m_file, SIGNAL(imageAvailable(QuillImageList)),
	  this, SLOT(fileLoaded()), Qt::QueuedConnection);
  QObject::connect(m_file, SIGNAL(removed()),
		   m_file, SLOT(deleteLater()), Qt::QueuedConnection);

  if (fileError()) {
    return;
  }

  m_file->setDisplayLevel(0);

  if (fileError()) {
    return;
  }

  emit sourceChanged();
}

void QuillItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
  Q_UNUSED(widget);

  if (!m_file) {
    return;
  }

  QImage image = m_file->image(0);

  if (!image.isNull()) {
    painter->drawImage(option->rect, image);
  }
}

void QuillItem::fileLoaded() {
  update();
}

bool QuillItem::fileError() {
  if (!m_file) {
    return true;
  }

  QuillError err = m_file->error();
  if (err.errorCode() != QuillError::NoError) {
    qWarning() << "Error loading file" << m_file->fileName() << err.errorData();

    QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection,
			      Q_ARG(QString, err.errorData()));
    m_file->deleteLater(); m_file = 0;
    return true;
  }

  return false;
}
