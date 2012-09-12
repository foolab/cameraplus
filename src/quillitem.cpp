#include "quillitem.h"
#include <QuillFile>
#include <QUrl>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDir>

QuillItem::QuillItem(QDeclarativeItem *parent) :
  QDeclarativeItem(parent), m_file(0), m_error(false) {

  setFlag(QGraphicsItem::ItemHasNoContents, false);

  static bool init = false;
  if (!init) {
    Quill::setPreviewLevelCount(1);
    Quill::setPreviewSize(0, QSize(854, 480)); // TODO:
    Quill::setMinimumPreviewSize(0, QSize(854, 480)); // TODO:
    Quill::setThumbnailExtension("jpeg"); // TODO:
    Quill::setThumbnailFlavorName(0, "screen");
    Quill::setBackgroundRenderingColor(Qt::black);
    QString tempPath(QDir::homePath() +  QDir::separator() + ".config" +
                     QDir::separator() + "quill" + QDir::separator() + "tmp");
    QDir().mkpath(tempPath);
    Quill::setTemporaryFilePath(tempPath);
    Quill::setDBusThumbnailingEnabled(true);
    Quill::setThumbnailCreationEnabled(true);

    init = true;
  }
}

QuillItem::~QuillItem() {
  delete m_file; m_file = 0;
}

void QuillItem::componentComplete() {
  QDeclarativeItem::componentComplete();

  recreate();
}

QUrl QuillItem::source() const {
  return m_url;
}

void QuillItem::setSource(const QUrl& src) {
  if (src == source()) {
    return;
  }

  m_url = src;

  if (isComponentComplete()) {
    recreate();
  }

  emit sourceChanged();
}

QString QuillItem::mimeType() const {
  return m_type;
}

void QuillItem::setMimeType(const QString& mime) {
  if (mimeType() == mime) {
    return;
  }

  m_type = mime;

  if (isComponentComplete()) {
    recreate();
  }

  emit mimeTypeChanged();
}

bool QuillItem::error() const {
  return m_error;
}

void QuillItem::recreate() {
  if (m_error) {
    m_error = false;
    emit errorChanged();
  }

  if (m_file) {
    m_file->deleteLater();
  }

  m_file = new QuillFile(m_url.toLocalFile(), m_type);

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
    qWarning() << "Error loading file" << m_file->fileName()
	       << "Code" << err.errorCode() << "Source" << err.errorSource();

    QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection,
			      Q_ARG(QString, err.errorData()));
    m_file->deleteLater(); m_file = 0;

    if (!m_error) {
      m_error = true;

      emit errorChanged();
    }

    return true;
  }

  return false;
}
