// -*- qml -*-

import QtQuick 1.1
import QtCamera 1.0

Rectangle {
        id: button
        property int mode: settings.mode

        color: "black"
        width: 60
        height: 120
        smooth: true
        radius: width
        border.width: 2
        border.color: "gray"

        visible: cam.running && cam.idle && !cameraMode.animationRunning

        Rectangle {
                id: highlighter
                width: parent.width
                height: parent.width
                color: mouse.pressed ? "lightblue" : "white"
                radius: parent.width
                y: mode == 1 ? video.y : image.y
        }

        Column {
                Image {
                        id: image
                        width: button.width
                        height: width
                        property string released: "icon-m-viewfinder-camera"
                        property string active: "icon-m-viewfinder-camera-selected"
                        source: mouse.pressed ? "image://theme/" + released : button.mode == 0 ? "image://theme/" + active : "image://theme/" + released
                }

                Image {
                        id: video
                        width: button.width
                        height: width
                        property string released: "icon-m-camera-video-record"
                        property string active: "icon-m-camera-video-selected"
                        source: mouse.pressed ? "image://theme/" + released : button.mode == 1 ? "image://theme/" + active : "image://theme/" + released
                }
        }

        MouseArea {
                anchors.fill: parent
                id: mouse
                drag.target: highlighter
                drag.axis: Drag.YAxis
                drag.minimumY: 0
                drag.maximumY: parent.height / 2
                onReleased: {

                        if (!drag.active) {
                                if (mode == 0) {
                                        settings.mode = Camera.VideoMode;
                                }
                                else {
                                        settings.mode = Camera.ImageMode;
                                }

                                return;
                        }

                        if (mouse.y >= video.y) {
                                settings.mode = Camera.VideoMode;
                        }
                        else {
                                settings.mode = Camera.ImageMode;
                        }
                }

                onPressed: {
                        var y = mouse.y - highlighter.height / 2;

                        if (y > drag.maximumY) {
                                y = drag.maximumY;
                        }

                        else if (y < drag.minimumY) {
                                y = 0;
                        }

                        highlighter.y = y
                }
        }
}
