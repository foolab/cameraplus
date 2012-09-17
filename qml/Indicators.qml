// -*- qml -*-
import QtQuick 1.1

Item {
        width: 48
        height: 200

        signal clicked

        MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: parent.clicked();
        }

        BorderImage {
                id: image
                anchors.fill: parent
                smooth: true
                source: mouse.pressed ? "image://theme/meegotouch-camera-settings-indicators-background-pressed" : "image://theme/meegotouch-camera-settings-indicators-background"
        }
}
