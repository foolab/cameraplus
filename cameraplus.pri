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
