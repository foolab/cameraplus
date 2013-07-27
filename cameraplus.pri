contains(MEEGO_EDITION, "harmattan") {
    message("Building for harmattan")
    DEFINES += HARMATTAN
    CONFIG += harmattan
} else {
# TODO: Assume nemo for now
    message("Building for nemo")
    DEFINES += NEMO
    CONFIG += nemo
}

equals(QT_MAJOR_VERSION, 4) {
    CONFIG += qt4
    DEFINES += QT4
}

equals(QT_MAJOR_VERSION, 5) {
    CONFIG += qt5
    DEFINES += QT5
}
