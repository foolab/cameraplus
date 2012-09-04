// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Page {
        property Camera cam: null
        property bool controlsVisible: true

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
