// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.1
import QtCamera 1.0
import CameraPlus 1.0

// TODO: metadata creator name, gps, geotags
// TODO: resolutions and aspect ratios
// TODO: postcapture
// TODO: battery low state
// TODO: disk space
// TODO: flash not ready
// TODO: focus, caf, ...
// TODO: indicators
// TODO: portrait/landscape
// TODO: record video in a hidden directory and then copy the video to avoid tracker indexing it.
// TODO: stop viewfinder in postcapture and settings pages ?
// TODO: resource policy
// TODO: prevent going to mass storage while recording and capturing
// TODO: sounds
// TODO: grid lines, face tracking
// TODO: complete settings pages
// TODO: stop camera properly when we get closed.
// TODO: select primary/secondary camera.
// TODO: disable debug builds.
// TODO: seems start gets called when we are shutting down
// TODO: seems start gets called twice when we are starting up if screen is locked or dimmed ?!

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

        function showError(msg) {
                error.text = msg;
                error.show();
        }

        FSMonitor {
                id: fileSystem
        }

        InfoBanner {
                id: error
        }

        Settings {
                id: settings
        }

        FileNaming {
                id: fileNaming
                imageSuffix: cam.imageSuffix
                videoSuffix: cam.videoSuffix
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
                                if (!cam.start()) {
                                        showError("Camera failed to start. Please restart the camera.");
                                }
                        }
                        else {
                                // This is a noop if camera is not idle so calling it will not hurt
                                cam.stop();
                        }
                }
        }

        Camera {
                onIdleChanged: {
                        if (idle && !platformWindow.active) {
                                stop();
                        }
                }

                onDeviceIdChanged: {
                        // TODO: is this needed ?
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
