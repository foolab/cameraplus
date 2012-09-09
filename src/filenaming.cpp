#include "filenaming.h"
#include <QDir>
#include <QDebug>
#include <QDate>
#include <QFile>

#define PATH QString("%1%2MyDocs%2cameraplus%2").arg(QDir::homePath()).arg(QDir::separator())

FileNaming::FileNaming(QObject *parent) :
  QObject(parent) {

}

FileNaming::~FileNaming() {

}

void FileNaming::setImageSuffix(const QString& suffix) {
  m_image = suffix;
}

void FileNaming::setVideoSuffix(const QString& suffix) {
  m_video = suffix;
}

QString FileNaming::imageFileName() {
  return fileName(m_image);
}

QString FileNaming::videoFileName() {
  return fileName(m_video);
}

QString FileNaming::fileName(const QString& suffix) {
  if (suffix.isEmpty()) {
    return QString();
  }

  if (!QDir::root().mkpath(PATH)) {
    qWarning() << "Failed to create" << PATH;
    return QString();
  }

  // Shamelessly stolen from Aura
  QDir dir(PATH);
  QString date = QDate::currentDate().toString("yyyyMMdd");

  QStringList filters(QString("*%1_*").arg(date));
  QStringList entries = dir.entryList(filters, QDir::Files, QDir::Name);

  int index = 0;

  if (!entries.isEmpty()) {
    QString name = QFile(entries.last()).fileName();
    index = name.section('_', 1, 1).section('.', 0, 0).toInt();
  }

  ++index;

  QString name = QString("%1%2_%3.%4").arg(PATH).arg(date).arg(QString().sprintf("%03i", index)).
    arg(suffix);

  return name;
}
