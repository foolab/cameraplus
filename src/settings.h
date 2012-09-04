// -*- c++ -*-

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class QSettings;

class Settings : public QObject {
  Q_OBJECT

  Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged);

public:
  Settings(QObject *parent = 0);
  ~Settings();

  int mode() const;
  void setMode(int mode);

signals:
  void modeChanged();

private:
  QSettings *m_settings;
};

#endif /* SETTINGS_H */
