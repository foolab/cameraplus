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

    ListView {
        anchors.centerIn: parent
        height: 150
        width: Math.min(parent.width, (count - 1) * 100)
        orientation: ListView.Horizontal
        model: plugins
        delegate: Rectangle {
            anchors {
                top: parent.top
                topMargin: 20
                bottom: parent.bottom
                bottomMargin: 20
            }

            width: visible ? 100 : 0
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
                anchors {
                    top: parent.top
                    horizontalCenter: parent.horizontalCenter
                }

                source: plugin.icon
                width: parent.width - 20
                height: width
            }

            CameraLabel {
                width: parent.width - 20
                height: 60
                text: plugin.name
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors {
                    bottom: parent.bottom
                    horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }
}
