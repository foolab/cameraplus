LIBS += -Wl,--as-needed

contains(MEEGO_EDITION, "harmattan") {
    message("Building for harmattan")
    DEFINES += HARMATTAN
    CONFIG += harmattan
} else {
    # We will assume sailfish for now since it's the only platform I am familiar with
    message("Building for sailfish")
    DEFINES += SAILFISH
    CONFIG += sailfish
}

equals(QT_MAJOR_VERSION, 4) {
    CONFIG += qt4
    DEFINES += QT4
}

equals(QT_MAJOR_VERSION, 5) {
    CONFIG += qt5
    DEFINES += QT5
}
