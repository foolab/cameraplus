// -*- c++ -*-

#ifndef QUILL_ITEM_H
#define QUILL_ITEM_H

#include <QDeclarativeItem>

class QuillFile;

class QuillItem : public QDeclarativeItem {
  Q_OBJECT

  Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged);

public:
  QuillItem(QDeclarativeItem *parent = 0);
  ~QuillItem();

  QUrl source() const;
  void setSource(const QUrl& src);

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

signals:
  void sourceChanged();
  void error(const QString& err);

private slots:
  void fileLoaded();
  bool fileError();

private:
  QuillFile *m_file;
};

#endif /* QUILL_ITEM_H */
