// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.1
import QtCamera 1.0
import CameraPlus 1.0
import QtMobility.systeminfo 1.2
import QtMobility.location 1.2

// TODO: resolutions and aspect ratios
// TODO: postcapture
// TODO: battery low state
// TODO: disk space
// TODO: flash not ready
// TODO: focus, caf, ...
// TODO: indicators
// TODO: portrait/landscape
// TODO: record video in a hidden directory and then copy the video to avoid tracker indexing it.
// TODO: stop viewfinder in settings pages ?
// TODO: prevent going to mass storage while recording and capturing
// TODO: sounds
// TODO: grid lines, face tracking, ambr
// TODO: complete settings pages
// TODO: stop camera properly when we get closed.
// TODO: select primary/secondary camera.
// TODO: disable debug builds.
// TODO: a way to get buffers to the application
// TODO: fcam like functionality (precise control over capture parameters).

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

        PositionSource {
                id: positionSource
                active: cam.running && settings.useGps
                onPositionChanged: geocode.search(position.coordinate.longitude, position.coordinate.latitude);
        }

        MetaData {
                id: metaData
                camera: cam
                manufacturer: deviceInfo.manufacturer
                model: deviceInfo.model
                country: geocode.country
                city: geocode.city
                suburb: geocode.suburb
                longitude: positionSource.position.coordinate.longitude
                longitudeValid: positionSource.position.longitudeValid && settings.useGps
                latitude: positionSource.position.coordinate.latitude
                latitudeValid: positionSource.position.latitudeValid && settings.useGps
                elevation: positionSource.position.coordinate.altitude
                elevationValid: positionSource.position.altitudeValid && settings.useGps
                orientation: orientation.orientation
                artist: settings.creatorName
                captureDirection: compass.direction
                captureDirectionValid: compass.directionValid
                horizontalError: positionSource.position.horizontalAccuracy
                horizontalErrorValid: positionSource.position.horizontalAccuracyValid && settings.useGps
                dateTimeEnabled: true
        }

        Orientation {
                id: orientation
                active: cam.running
        }

        Compass {
                id: compass
                active: cam.running
        }

        ReverseGeocode {
                id: geocode
                active: cam.running && settings.useGps && settings.useGeotags
        }

        CameraResources {
                id: resourcePolicy
                onAcquiredChanged: {
                        if (resourcePolicy.acquired) {
                                // TODO:
                        }
                        else {
                                // TODO: We need a way to force a stop.
                        }
                }
        }

        DeviceInfo {
                id: deviceInfo
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

        Camera {
/*
                onDeviceIdChanged: {
                        // TODO: is this needed ?
                        if (platformWindow.active) {
                                cam.start();
                        }
                }
*/
                id: cam
                anchors.fill: parent

                onError: {
                        console.log("Camera error (" + code + "): " + message + " " + debug);
                        showError(qsTr("Camera error. Please restart the application."));
                        cam.stop();
                }

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

        Iso {
                id: iso
                camera: cam
                value: ready ? settings.imageIso : 0
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
