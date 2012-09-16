// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0
import CameraPlus 1.0

CameraPage {
        id: page

        policyMode: CameraResources.Video

        controlsVisible: recording.visible && cam.running && !standbyWidget.visible

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
                visible: controlsVisible
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 20
                anchors.leftMargin: 20
                opacity: 0.5
        }

        VideoSceneButton {
                id: scene
                visible: controlsVisible && !videoMode.recording
                anchors.top: torch.bottom
                anchors.left: parent.left
                anchors.topMargin: 10
                anchors.leftMargin: 20
        }

        EvCompButton {
                id: evComp
                visible: controlsVisible
                anchors.top: scene.bottom
                anchors.left: parent.left
                anchors.topMargin: 10
                anchors.leftMargin: 20
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
