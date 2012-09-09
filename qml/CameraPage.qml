// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Page {

        property alias standbyWidget: standby

        Rectangle {
                // TODO: color
                // TODO: fade out transition
                // TODO: there is a toolbar visible on the first startup
                id: standby
                color: "black"
                anchors.fill: parent
                visible: !platformWindow.active
                Image {
                        source: "image://theme/icon-l-camera-standby"
                        anchors.centerIn: parent
                }
        }

        property Camera cam: null
        property bool controlsVisible: cam.running && !standby.visible

        anchors.fill: parent

        property alias previewAnimationRunning: preview.animationRunning

        function setPreview(image) {
                preview.setPreview(image);
        }

        ModeButton {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.bottomMargin: 20
        }

        PreviewImage {
                id: preview
        }

        ZoomSlider {
                id: zoom
                camera: cam
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.horizontalCenter: parent.horizontalCenter
                visible: controlsVisible
        }
}
