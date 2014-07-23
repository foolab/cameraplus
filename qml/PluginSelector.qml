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

Rectangle {
    id: pluginSelector
    anchors.fill: parent
    color: "black"
    border.color: "gray"
    radius: 20

    Flow {
        anchors.centerIn: parent

        Repeater {
            model: plugins

            delegate: Rectangle {
                width: visible ? pluginSelector.width / 3 : 0
                height: visible ? pluginSelector.height / 2 : 0
                color: mouse.pressed ? cameraStyle.pressedColor : "transparent"
                visible: plugin.uuid != activePlugin.uuid && ((settings.device == 0 && plugin.primaryCameraSupported) || (settings.device == 1 && plugin.secondaryCameraSupported))

                MouseArea {
                    id: mouse
                    anchors.fill: parent
                    onClicked: {
                        settings.plugin = plugin.uuid
                        pluginSelector.parent.hide()
                    }
                }

                Image {
                    id: icon
                    anchors {
                        left: parent.left
                        leftMargin: 20
                        verticalCenter: parent.verticalCenter
                    }

                    source: plugin.icon
                    width: parent.height
                    height: width
                }

                CameraLabel {
                    width: 120
                    height: parent.height
                    text: plugin.name
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors {
                        left: icon.right
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }
}
