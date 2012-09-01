// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

CameraPage {
        id: page

        controlsVisible: capture.visible

        Button {
                id: capture
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                iconSource: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-shutter.png"
                width: 75
                height: 75
                opacity: 0.5
                onClicked: imageMode.capture("/home/developer/foo.png");
                visible: imageMode.canCapture && !cameraMode.animationRunning && !previewAnimationRunning
        }

        ImageMode {
                id: imageMode
                camera: cam
                onPreviewAvailable: page.setPreview(preview);
                nightMode: scene.value == Scene.Night
        }

        FlashButton {
                id: flash
                visible: capture.visible
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 20
                anchors.leftMargin: 20
        }

        ImageSceneButton {
                id: scene
                visible: capture.visible
                anchors.top: flash.bottom
                anchors.left: parent.left
                anchors.topMargin: 10
                anchors.leftMargin: 20
        }

        EvCompButton {
                id: evComp
                visible: capture.visible
                anchors.top: scene.bottom
                anchors.left: parent.left
                anchors.topMargin: 10
                anchors.leftMargin: 20
        }

        // TODO: filenaming.
        // TODO: metadata
}
