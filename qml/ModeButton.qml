// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2014 Mohammed Sameer <msameer@foolab.org>
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


import QtQuick 2.0
import QtCamera 1.0

CameraToolIcon {
    property alias drag: mouse.drag

    width: 75
    height: 75
    opacity: 0.5

    border.color: cameraStyle.borderColor
    border.width: 1
    radius: width / 2

    Behavior on x {
        PropertyAnimation { duration: 200 }
    }

    MouseArea {
        anchors.fill: parent
        id: mouse

        property real __pos
        property int __dir
        onPressed: {
            __pos = mapToItem(parent.parent, mouse.x, mouse.y).x
            __dir = 0
        }

        onPositionChanged: {
            var pos = mapToItem(parent.parent, mouse.x, mouse.y).x;
            if (pos > __pos) {
                __dir = 1
            } else if (pos < __pos) {
                __dir = -1
            }

            __pos = pos
        }


        drag {
            target: parent
            axis: Drag.XAxis
            filterChildren: true

            onActiveChanged: {
                if (!mouse.drag.active) {
                    if (__dir == -1) {
                        parent.x = mouse.drag.maximumX
                    } else if (__dir == 1) {
                        parent.x = mouse.drag.minimumX
                    }
                }
            }
        }

        onClicked: parent.clicked()
    }

    color: mouse.pressed ? cameraStyle.pressedColor : cameraStyle.releasedColor

    iconSource: cameraTheme.modeButtonIconId

    onClicked: {
        if (plugins.count <= 1) {
            return;
        } else if (plugins.count == 2) {
            // just switch
            var plugin = plugins.at(0)
            if (plugin.uuid != settings.plugin) {
                settings.plugin = plugin.uuid;
            } else {
                plugin = plugins.at(1)
                settings.plugin = plugin.uuid;
            }
        } else {
            if (activePlugin.uuid.search(".image") != -1) {
                // switch to video
                settings.plugin = "org.foolab.cameraplus.video"
            } else {
                // switch to image
                settings.plugin = "org.foolab.cameraplus.image"
            }
        }
    }
}
