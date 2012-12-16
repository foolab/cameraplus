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

// TODO: keep reticle within bounds
// TODO: move reticle within bounds if resolution changes

MouseArea {
        property int cafStatus: AutoFocus.None
        property int status: AutoFocus.None
        property variant topLeft: mapFromItem(cam, cam.renderArea.x, cam.renderArea.y)
        property variant bottomRight: mapFromItem(cam, cam.renderArea.x + cam.renderArea.width, cam.renderArea.y + cam.renderArea.height)
        id: mouse

        // A 100x100 central "rectangle"
        property variant centerRect: Qt.rect((mouse.width / 2 - 50), (mouse.height / 2) - 50, 100, 100);

        property alias touchMode: reticle.touchMode

        x: topLeft.x
        y: topLeft.y
        width: bottomRight.x - topLeft.x
        height: bottomRight.y - topLeft.y

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

        function moveRect(x, y) {
                // TODO: don't put reticle outside area
                x = x - (reticle.width / 2)
                y = y - (reticle.height / 2)

                reticle.x = x;
                reticle.y = y;
        }

        function moveToCenterIfNeeded(x, y) {
                if (x >= centerRect.x && y >= centerRect.y &&
                    x <= centerRect.x + centerRect.width &&
                    y <= centerRect.y + centerRect.height) {
                        reticle.x = reticle.center.x
                        reticle.y = reticle.center.y
                }
        }

        onPressed: moveRect(mouse.x, mouse.y);
        onPositionChanged: moveRect(mouse.x, mouse.y);

        onReleased: moveToCenterIfNeeded(mouse.x, mouse.y);

        onXChanged: {
                // TODO:
//                moveRect(reticle.x, reticle.y);
        }

        FocusRectangle {
                id: reticle
                property variant center: Qt.point((mouse.width - width) / 2, (mouse.height - height) / 2);
                property bool touchMode: !(reticle.x == center.x && reticle.y == center.y)

                scale: mouse.pressed ? 0.6 :  touchMode ? 0.8 : 1.0

                width: 250
                height: 150
                x: center.x
                y: center.y

                color: predictColor(cafStatus, status);

                onXChanged: {
                        if (mouse.pressed) {
                                return;
                        }

//                        console.log(x);
//                        console.log(x);
                }
        }

        Timer {
                interval: 500
                running: status == AutoFocus.Running
                triggeredOnStart: true
                repeat: true
                onTriggered: reticle.visible = !reticle.visible
        }
}
