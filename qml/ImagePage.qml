// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0
import CameraPlus 1.0

CameraPage {
        id: page

        policyMode: CameraResources.Image
        controlsVisible: capture.visible && cam.running && !standbyWidget.visible

        Button {
                id: capture
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                iconSource: "image://theme/icon-m-camera-shutter"
                width: 75
                height: 75
                opacity: 0.5
                onClicked: {
                        if (!fileSystem.available) {
                                showError(qsTr("Camera cannot capture images in mass storage mode."));
                        }
                        else if (!imageMode.capture(fileNaming.imageFileName())) {
                                showError(qsTr("Failed to capture image. Please restart the camera."));
                        }
                }

                visible: imageMode.canCapture && !cameraMode.animationRunning && !previewAnimationRunning && cam.running
        }

        ImageMode {
                id: imageMode
                camera: cam
                onPreviewAvailable: page.setPreview(preview);
        }

        FlashButton {
                id: flash
                visible: controlsVisible
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 20
                anchors.leftMargin: 20
        }

        ImageSceneButton {
                id: scene
                visible: controlsVisible
                anchors.top: flash.bottom
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

        Indicators {
                visible: controlsVisible
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 20

                onClicked: openFile("ImageSettingsPage.qml");
//                Image {
///                        
//                }
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
                visible: controlsVisible
        }

        // TODO: metadata
}
