// -*- c++ -*-

#ifndef FILE_NAMING_H
#define FILE_NAMING_H

#include <QObject>

class FileNaming : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString imageSuffix WRITE setImageSuffix);
  Q_PROPERTY(QString videoSuffix WRITE setVideoSuffix);

public:
  FileNaming(QObject *parent = 0);
  ~FileNaming();

  void setImageSuffix(const QString& suffix);
  void setVideoSuffix(const QString& suffix);

  Q_INVOKABLE QString imageFileName();
  Q_INVOKABLE QString videoFileName();

private:
  QString fileName(const QString& suffix);

  QString m_image;
  QString m_video;
};

#endif /* FILE_NAMING_H */
