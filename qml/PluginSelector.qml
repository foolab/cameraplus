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
    width: flow.width + (radius * 2)
    color: cameraStyle.backgroundColor
    border.color: cameraStyle.borderColor
    radius: 20

    Flow {
        id: flow
        flow: Flow.TopToBottom

        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            margins: cameraStyle.padding
        }

        Repeater {
            model: plugins

            delegate: Rectangle {
                width: visible ? label.width + label.anchors.leftMargin + icon.width : 0
                height: visible ? cameraStyle.pluginSelectorDelagateHeight : 0
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
                    anchors.left: parent.left
                    source: plugin.icon
                    width: parent.height
                    height: width
                    anchors.verticalCenter: parent.verticalCenter
                }

                CameraLabel {
                    id: label
                    height: parent.height
                    text: plugin.name
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter

                    anchors {
                        left: icon.right
                        leftMargin: cameraStyle.padding
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }
}
