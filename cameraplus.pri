contains(MEEGO_EDITION, "harmattan") {
    message("Building for harmattan")
    DEFINES += HARMATTAN
    CONFIG += harmattan
}
