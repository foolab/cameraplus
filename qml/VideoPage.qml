// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0
import CameraPlus 1.0

CameraPage {
        id: page

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
                        if (!videoMode.recording) {
                                if (!fileSystem.available) {
                                        showError(qsTr("Camera cannot record videos in mass storage mode."));
                                }
                                else if (!videoMode.startRecording(fileNaming.videoFileName())) {
                                        showError(qsTr("Failed to record video. Please restart the camera."));
                                }
                        }
                        else {
                                videoMode.stopRecording();
                        }
                }

                visible: (videoMode.recording || videoMode.canCapture) && !cameraMode.animationRunning && !previewAnimationRunning && !standbyWidget.visible
        }

        Connections {
                target: platformWindow
                onActiveChanged: {
                        if (!platformWindow.active && videoMode.recording) {
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
