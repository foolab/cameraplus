// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


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
                y: mode == Camera.VideoMode ? video.y : image.y
        }

        Column {
                Image {
                        id: image
                        width: button.width
                        height: width
                        property string released: "icon-m-viewfinder-camera"
                        property string active: "icon-m-viewfinder-camera-selected"
                        source: mouse.pressed ? "image://theme/" + released : button.mode == Camera.ImageMode ? "image://theme/" + active : "image://theme/" + released
                }

                Image {
                        id: video
                        width: button.width
                        height: width
                        property string released: "icon-m-camera-video-record"
                        property string active: "icon-m-camera-video-selected"
                        source: mouse.pressed ? "image://theme/" + released : button.mode == Camera.VideoMode ? "image://theme/" + active : "image://theme/" + released
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
                                if (mode == Camera.ImageMode) {
                                        settings.mode = Camera.VideoMode;
                                }
                                else if (mode == Camera.VideoMode) {
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
