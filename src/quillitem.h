// -*- c++ -*-

#ifndef QUILL_ITEM_H
#define QUILL_ITEM_H

#include <QDeclarativeItem>

class QuillFile;

class QuillItem : public QDeclarativeItem {
  Q_OBJECT

  Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged);
  Q_PROPERTY(QString mimeType READ mimeType WRITE setMimeType NOTIFY mimeTypeChanged);
  Q_PROPERTY(bool error READ error NOTIFY errorChanged);

public:
  QuillItem(QDeclarativeItem *parent = 0);
  ~QuillItem();

  QUrl source() const;
  void setSource(const QUrl& src);

  QString mimeType() const;
  void setMimeType(const QString& mime);

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

  virtual void componentComplete();

  bool error() const;

signals:
  void sourceChanged();
  void error(const QString& err);
  void mimeTypeChanged();
  void errorChanged();

private slots:
  void fileLoaded();
  bool fileError();

private:
  void recreate();

  QuillFile *m_file;
  QUrl m_url;
  QString m_type;
  bool m_error;
};

#endif /* QUILL_ITEM_H */
