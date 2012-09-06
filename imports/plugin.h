// -*- c++ -*-

#ifndef PLUGIN_H
#define PLUGIN_H

class QDeclarativeEngine;

class Plugin {
public:
  static void registerTypes(QDeclarativeEngine *engine);
};

#endif /* PLUGIN_H */
