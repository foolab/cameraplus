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
                nightMode: scene.sceneValue == Scene.Night
        }

        FlashButton {
                id: flash
                visible: capture.visible
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 20
                anchors.leftMargin: 20
        }

        SceneButton {
                id: scene
                visible: capture.visible
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 20 + 64 + 10
                anchors.leftMargin: 20
        }

        Selector {
                id: evComp
                visible: capture.visible
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 20 + 64 + 10 + 64 + 10
                anchors.leftMargin: 20
                timerConstraints: slider.pressed
                widget: Slider {
                        id: slider
                        orientation: Qt.Horizontal
//                        anchors.horizontalCenter: parent.horizontalCenter
                }
        }

        // TODO: filenaming.
        // TODO: metadata
}
