// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0
import CameraPlus 1.0
import "data.js" as Data

// TODO: video recording indicators.
// TODO: stop recording when battery low
// TODO: stop recording when disk is low
CameraPage {
        id: page

        policyMode: CameraResources.Video

        controlsVisible: videoControlsVisible && !videoMode.recording
        property bool videoControlsVisible: recording.visible && cam.running && !standbyWidget.visible

        orientationLock: PageOrientation.LockLandscape

        DisplayState {
                inhibitDim: videoMode.recording
        }

        Button {
                id: recording
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                iconSource: "image://theme/icon-m-camera-video-record"
                width: 75
                height: 75
                opacity: 0.5

                onClicked: {
                        if (!fileSystem.available) {
                                showError(qsTr("Camera cannot record videos in mass storage mode."));
                                return;
                        }

                        // We only toggle the mode to video recording so
                        // policy can acquire the needed resources

                        if (policyMode == CameraResources.Video) {
                                policyMode = CameraResources.Recording;
                        }
                        else if (videoMode.recording) {
                                // We just ask to stop video.
                                videoMode.stopRecording();
                        }
                }

                Connections {
                        target: videoMode
                        onRecordingChanged: {
                                if (!videoMode.recording) {
                                        policyMode = CameraResources.Video;
                                }
                        }
                }

                Connections {
                        target: resourcePolicy
                        onAcquiredChanged: {
                                if (resourcePolicy.acquired && policyMode == CameraResources.Recording) {
                                        metaData.setMetaData();
                                        if (!videoMode.startRecording(fileNaming.videoFileName())) {
                                                showError(qsTr("Failed to record video. Please restart the camera."));
                                                policyMode = CameraResources.Video
}
                                }
                        }
                }

                visible: (videoMode.recording || videoMode.canCapture) && !cameraMode.animationRunning && !previewAnimationRunning && !standbyWidget.visible
        }

        Connections {
                target: Qt.application
                onActiveChanged: {
                        if (!Qt.application.active && videoMode.recording) {
                                videoMode.stopRecording();
                        }
                }
        }

        VideoMode {
                id: videoMode
                camera: cam
                onPreviewAvailable: {
                        if (!standbyWidget.visible) {
                                page.setPreview(preview);
                        }
                }
        }

        VideoTorchButton {
                id: torch
                visible: videoControlsVisible
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 20
                anchors.leftMargin: 20
                opacity: 0.5
        }

        VideoSceneButton {
                id: scene
                visible: controlsVisible
                anchors.top: torch.bottom
                anchors.left: parent.left
                anchors.topMargin: 10
                anchors.leftMargin: 20
        }

        EvCompButton {
                id: evComp
                visible: videoControlsVisible
                anchors.top: scene.bottom
                anchors.left: parent.left
                anchors.topMargin: 10
                anchors.leftMargin: 20
        }

        MouseArea {
                id: indicators
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 20
                width: 48
                height: col.height
                onClicked: openFile("VideoSettingsPage.qml");
                visible: controlsVisible

                BorderImage {
                        id: image
                        anchors.fill: parent
                        smooth: true
                        source: indicators.pressed ? "image://theme/meegotouch-camera-settings-indicators-background-pressed" : "image://theme/meegotouch-camera-settings-indicators-background"
                }

                Column {
                        id: col
                        width: parent.width
                        spacing: 5

                        Indicator {
                                id: resolutionIndicator
                                // TODO:
                        }

                        Indicator {
                                id: wbIndicator
                                source: "image://theme/" + Data.wbIcon(settings.videoWhiteBalance) + "-screen"
                                visible: settings.videoWhiteBalance != WhiteBalance.Auto
                        }

                        Indicator {
                                id: cfIndicator
                                source: "image://theme/" + Data.cfIcon(settings.videoColorFilter) + "-screen"
                                visible: settings.videoColorFilter != ColorTone.Normal
                        }

                        Indicator {
                                id: gpsIndicator
                                visible: settings.useGps
                                source: "image://theme/icon-m-camera-location"

                                PropertyAnimation on opacity  {
                                        easing.type: Easing.OutSine
                                        loops: Animation.Infinite
                                        from: 0.2
                                        to: 1.0
                                        duration: 1000
                                        running: settings.useGps && !positionSource.position.longitudeValid
                                        alwaysRunToEnd: true
                                }
                        }
                }
        }

        Button {
                id: cameraRoll
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.topMargin: 20
                anchors.rightMargin: 20
                width: 56
                height: 56

                opacity: 0.5
                iconSource: "image://theme/icon-m-camera-roll"
                onClicked: openFile("PostCapturePage.qml");
                visible: controlsVisible && !videoMode.recording
        }
}
