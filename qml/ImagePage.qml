// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

CameraPage {
        id: page

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
                // TODO:
                onClicked: imageMode.capture("/home/developer/foo.png");
                visible: imageMode.canCapture && !cameraMode.animationRunning && !previewAnimationRunning
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

        // TODO: filenaming.
        // TODO: metadata
}
