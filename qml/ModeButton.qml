// -*- qml -*-

import QtQuick 1.1
import QtCamera 1.0

Rectangle {
        id: button
        property int mode: Camera.ImageMode

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
//                        anchors.top: parent.top
                        property string released: "icon-m-viewfinder-camera.png"
                        property string active: "icon-m-viewfinder-camera-selected.png"
                        source: mouse.pressed ? "/usr/share/themes/blanco/meegotouch/icons/" + released : button.mode == 0 ? "/usr/share/themes/blanco/meegotouch/icons/" + active : "/usr/share/themes/blanco/meegotouch/icons/" + released
                }

                Image {
                        id: video
                        width: button.width
                        height: width
                        property string released: "icon-m-camera-video-record.png"
                        property string active: "icon-m-camera-video-selected.png"
                        source: mouse.pressed ? "/usr/share/themes/blanco/meegotouch/icons/" + released : button.mode == 1 ? "/usr/share/themes/blanco/meegotouch/icons/" + active : "/usr/share/themes/blanco/meegotouch/icons/" + released
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
                        mode = Camera.VideoMode;
//                        highlighter.y = video.y;
                        }
                        else {
                                mode = Camera.ImageMode;
//                                highlighter.y = image.y;
                        }
                        return;
                        }

//                        var pos = 0;
                        if (mouse.y >= video.y) {
                                mode = Camera.VideoMode;
//                                pos = video.y;
                        }
                        else {
                                mode = Camera.ImageMode;
//                                pos = image.y;
                        }

//                        highlighter.y = pos;
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

/*
        ListView {
                anchors.fill: parent
                interactive: false

                model: ListModel {
                        id: model
                        ListElement {
                                mode: 0
                                released: "icon-m-viewfinder-camera.png"
                                active: "icon-m-viewfinder-camera-selected.png"
                        }

                        ListElement {
                                mode: 1
                                released: "icon-m-camera-video-record.png"
                                active: "icon-m-camera-video-selected.png"
                        }
                }

                delegate: Rectangle {
                        width: button.width
                        height: button.width
                        radius: parent.width
                        smooth: true
                        color: mouse.pressed ? "black" : button.mode == mode ? "white" : "black"
                        Image {
                                z: 10
                                id: image
                                source: mouse.pressed ? "/usr/share/themes/blanco/meegotouch/icons/" + released : button.mode == mode ? "/usr/share/themes/blanco/meegotouch/icons/" + active : "/usr/share/themes/blanco/meegotouch/icons/" + released
                                anchors.fill: parent
                        }
                }
        }
               */
}
