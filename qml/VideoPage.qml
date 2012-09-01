// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

CameraPage {
        id: page

        controlsVisible: recording.visible

        orientationLock: PageOrientation.LockLandscape

        Button {
                id: recording
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                iconSource: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-video-record.png"
                width: 75
                height: 75
                opacity: 0.5
                onClicked: {
                        if (!videoMode.recording) {
                                videoMode.startRecording("/home/developer/foo.mp4");
                        }
                        else {
                                videoMode.stopRecording();
                        }
                }

                visible: (videoMode.recording || videoMode.canCapture) && !cameraMode.animationRunning && !previewAnimationRunning
        }

        VideoMode {
                id: videoMode
                camera: cam
                onPreviewAvailable: page.setPreview(preview);
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
                visible: controlsVisible
                anchors.top: torch.bottom
                anchors.left: parent.left
                anchors.topMargin: 10
                anchors.leftMargin: 20
        }
}
