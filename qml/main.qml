// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0
import CameraPlus 1.0

// TODO: metadata creator name, gps, geotags
// TODO: resolutions and aspect ratios
// TODO: postcapture
// TODO: file naming
// TODO: battery low state
// TODO: disk space
// TODO: flash not ready
// TODO: focus, caf, ...
// TODO: indicators
// TODO: portrait/landscape
// TODO: record in a hidden directory and then copy the video to avoid tracker indexing it.

PageStackWindow {
        id: root

        property alias dimmer: camDimmer

        showStatusBar: false

        Component.onCompleted: {
                theme.inverted = true;
                if (settings.mode == 0) {
                        openFile("ImagePage.qml");
                }
                else {
                        openFile("VideoPage.qml");
                }
        }

        Settings {
                id: settings
        }

        // Stolen from https://qt.gitorious.org/qt-components/qt-components/blobs/master/examples/meego/QmlComponentGallery/qml/ListPage.qml
        function replacePage(file) {
                var component = Qt.createComponent(file)

                if (component.status == Component.Ready) {
                        pageStack.replace(component, {cam: cam}, true);
                }
                else {
                        console.log("Error loading component:", component.errorString());
                }
        }

        function openFile(file) {
                var component = Qt.createComponent(file)

                if (component.status == Component.Ready) {
                        pageStack.push(component, {cam: cam});
                }
                else {
                        console.log("Error loading component:", component.errorString());
                }
        }

        platformStyle: PageStackWindowStyle {
                // TODO: Hack
                background: " "
        }

        Connections {
                target: platformWindow
                onActiveChanged: {
                        if (platformWindow.active) {
                                cam.start();
                        }
                        else {
                                // TODO: only stop if idle.
                                cam.stop();
                        }
                }
        }

        Camera {
                onDeviceIdChanged: {
                        if (platformWindow.active) {
                                cam.start();
                        }
                }

                id: cam
                anchors.fill: parent

                // TODO: hardcoding device id
                Component.onCompleted: { cam.deviceId = 0; mode = settings.mode; }

                // TODO: Hack
                z: -1

                Rectangle {
                        id: camDimmer
                        z: 1
                        anchors.fill: parent
                        opacity: 0
                        color: "black"
                }
        }

        Scene {
                id: sceneController
                camera: cam
                value: ready ? camera.mode == Camera.VideoMode ? settings.videoSceneMode : settings.imageSceneMode : 0
        }

        ColorTone {
                id: colorToneController
                camera: cam
                value: ready ? camera.mode == Camera.VideoMode ? settings.videoColorFilter : settings.imageColorFilter : 0
        }

        WhiteBalance {
                id: whiteBalanceController
                camera: cam
                value: ready ? camera.mode == Camera.VideoMode ? settings.videoWhiteBalance : settings.imageWhiteBalance : 0
        }

        ModeController {
                id: cameraMode
                cam: cam
                dimmer: root.dimmer
        }

        Connections {
                target: cam
                onModeChanged: {
                        if (cam.mode == Camera.VideoMode) {
                                replacePage("VideoPage.qml");
                        }
                        else {
                                replacePage("ImagePage.qml");
                        }
                }
        }
}
