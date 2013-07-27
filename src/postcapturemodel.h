// -*- c++ -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef POST_CAPTURE_MODEL_H
#define POST_CAPTURE_MODEL_H

#include <QAbstractListModel>
#include <QUrl>

class QSparqlConnection;
class PostCaptureModelItem;
class QDateTime;
class Quad;
class QSparqlQuery;
class QSparqlResultRow;

class PostCaptureModel : public QAbstractListModel {
  Q_OBJECT

  Q_PROPERTY(QString manufacturer READ manufacturer WRITE setManufacturer NOTIFY manufacturerChanged);
  Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged);

  typedef enum {
    Item = Qt::UserRole + 1
  } Roles;

public:
  PostCaptureModel(QObject *parent = 0);
  ~PostCaptureModel();

  virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  QString manufacturer() const;
  void setManufacturer(const QString& manufacturer);

  QString model() const;
  void setModel(const QString& model);

  QHash<int, QByteArray> roleNames() const;

signals:
  void error(const QString& msg);

  void manufacturerChanged();
  void modelChanged();

public slots:
  void reload();
  void remove(QObject *item);

private slots:
  void dataReady(int totalCount);
  void graphUpdated(const QString& className, const QList<Quad>& deleted,
		    const QList<Quad>& inserted);

private:
  void addRow(PostCaptureModelItem *item);
  void exec(QSparqlQuery& query);

  QSparqlConnection *m_connection;
  QString m_manufacturer;
  QString m_model;

  QList<PostCaptureModelItem *> m_items;
  QHash<int, PostCaptureModelItem *> m_hash;

  bool m_connected;
};

class PostCaptureModelItem : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString type READ type NOTIFY typeChanged);
  Q_PROPERTY(QUrl url READ url NOTIFY urlChanged);
  Q_PROPERTY(QString created READ created NOTIFY createdChanged);
  Q_PROPERTY(QString title READ title NOTIFY titleChanged);
  Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged);
  Q_PROPERTY(QString mimeType READ mimeType NOTIFY mimeTypeChanged);
  Q_PROPERTY(bool available READ available NOTIFY availableChanged);
  Q_PROPERTY(QString lastModified READ lastModified NOTIFY lastModifiedChanged);
  Q_PROPERTY(unsigned trackerId READ trackerId CONSTANT);
  Q_PROPERTY(bool favorite READ favorite WRITE setFavorite NOTIFY favoriteChanged);

public:
  PostCaptureModelItem(const QSparqlResultRow& row, QObject *parent = 0);

  ~PostCaptureModelItem() {

  }

  void update(PostCaptureModelItem *other);

  QString type() const;
  QUrl url() const;
  QString created() const;
  QString title() const;
  QString fileName() const;
  QString mimeType() const;
  bool available() const;
  QString lastModified() const;
  unsigned trackerId() const;

  void setFavorite(bool add);
  bool favorite() const;

signals:
  void typeChanged();
  void urlChanged();
  void createdChanged();
  void titleChanged();
  void fileNameChanged();
  void mimeTypeChanged();
  void availableChanged();
  void lastModifiedChanged();
  void favoriteChanged();

private:
  QString formatDateTime(const QDateTime& dt) const;
  QVariant value(const QString& id, const QVariant& def = QVariant()) const;

  QVariantMap m_data;
};

#endif /* POST_CAPTURE_MODEL_H */
