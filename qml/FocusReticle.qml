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
import CameraPlus 1.0

// TODO: I've seen the reticle color changing to red while dragging it but failed to reproduce :(
MouseArea {
        property int cafStatus: AutoFocus.None
        property int status: AutoFocus.None
        id: mouse

        // A 100x100 central "rectangle"
        property variant centerRect: Qt.rect((mouse.width / 2 - 50), (mouse.height / 2) - 50, 100, 100);

        property alias touchMode: reticle.touchMode

        x: cam.renderArea.x
        y: cam.renderArea.y
        width: cam.renderArea.width
        height: cam.renderArea.height

        // Changing mode (which implies changing pages) will not reset ROI thus we do it here
        Component.onCompleted: cam.autoFocus.setRegionOfInterest(Qt.rect(0, 0, 0, 0));

/*
        // This is for debugging
        Rectangle {
                color: "blue"
                opacity: 0.2
                anchors.fill: parent
        }

        Rectangle {
                color: "red"
                opacity: 0.4
                x: centerRect.x
                y: centerRect.y
                width: centerRect.width
                height: centerRect.height
        }
*/
        drag.target: reticle
        drag.axis: Drag.XandYAxis
        drag.minimumX: 0 - (0.1 * reticle.width)
        drag.minimumY: 0 - (0.1 * reticle.height)
        drag.maximumX: width - reticle.width + (0.1 * reticle.width)
        drag.maximumY: height - reticle.height + (0.1 * reticle.height)

        onStatusChanged: {
                if (status != AutoFocus.Running) {
                        reticle.visible = true;
                }
        }

        function predictColor(caf, status) {
                if (status == AutoFocus.Success) {
                        return "steelblue";
                }
                else if (status == AutoFocus.Fail) {
                        return "red";
                }
                else if (status == AutoFocus.Running) {
                        return "white";
                }
                else if (caf == AutoFocus.Success) {
                        return "steelblue";
                }
                else {
                        return "white";
                }
        }

        function moveReticle(x, y) {
                var xPos = x - ((reticle.width * 1) / 2);
                var yPos = y - ((reticle.height * 1) / 2);
                x = Math.min(Math.max(xPos, drag.minimumX), drag.maximumX);
                y = Math.min(Math.max(yPos, drag.minimumY), drag.maximumY);
                reticle.x = xPos;
                reticle.y = yPos;
        }

        function moveToCenterIfNeeded(x, y) {
                if (x >= centerRect.x && y >= centerRect.y &&
                    x <= centerRect.x + centerRect.width &&
                    y <= centerRect.y + centerRect.height) {
                        reticle.x = reticle.center.x
                        reticle.y = reticle.center.y
                }
        }

        function setRegionOfInterest() {
                if (!reticle.touchMode) {
                        cam.autoFocus.setRegionOfInterest(Qt.rect(0, 0, 0, 0));
                        return;
                }

                // take into account scale:
                var x = reticle.x + (reticle.width * 0.1);
                var y = reticle.y + (reticle.height * 0.1);

                var width = reticle.width * 0.8;
                var height = reticle.height * 0.8;

                // in terms of video resolution:
                x = (cam.videoResolution.width * x) / mouse.width;
                width = (cam.videoResolution.width * width) / mouse.width;
                y = (cam.videoResolution.height * y) / mouse.height;
                height = (cam.videoResolution.height * height) / mouse.height;

                // Translate to normalized coordinates (1x1 square) as expected by our C++ backend
                x = x / cam.videoResolution.width;
                width = width / cam.videoResolution.width;
                y = y / cam.videoResolution.height;
                height = height / cam.videoResolution.height;

                cam.autoFocus.setRegionOfInterest(Qt.rect(x, y, width, height));
        }

        onReleased: {
                moveToCenterIfNeeded(mouse.x, mouse.y);
                setRegionOfInterest();
        }

        onPressed: {
                moveReticle(mouse.x, mouse.y);
        }

        FocusRectangle {
                id: reticle
                property variant center: Qt.point((mouse.width - width) / 2, (mouse.height - height) / 2);
                property bool touchMode: !(reticle.x == center.x && reticle.y == center.y)

                scale: mouse.pressed ? 0.6 : touchMode ? 0.8 : 1.0

                width: 250
                height: 150
                x: center.x
                y: center.y

                color: predictColor(cafStatus, status);
        }

        Timer {
                interval: 500
                running: status == AutoFocus.Running
                triggeredOnStart: true
                repeat: true
                onTriggered: reticle.visible = !reticle.visible
        }
}
