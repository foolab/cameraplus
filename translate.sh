#!/bin/sh

lupdate  -extensions cc,cpp,js,qml,h `find qml src declarative lib -name "*.cc" -o -name "*.cpp"  -o -name "*.js" -o -name "*.qml" -o -name "*.h"` -ts cameraplus.ts
