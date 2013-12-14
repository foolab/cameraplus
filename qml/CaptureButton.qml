// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
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
    id: button

    property alias iconSource: icon.source
    property CameraButtonStyle platformStyle: CameraButtonStyle {}

    property alias pressed: mouse.pressed
    property alias mouseX: mouse.mouseX
    property alias mouseY: mouse.mouseY

    signal clicked
    signal exited

    color: mouse.pressed ? platformStyle.pressedColor : platformStyle.releasedColor
    border.color: platformStyle.borderColor
    border.width: 1
    radius: width / 3

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: button.clicked()
        onExited: button.exited()
    }

    Image {
        id: icon
        anchors.centerIn: parent
        width: parent.width * 0.75
        height: parent.height * 0.75
        anchors.verticalCenter: parent.verticalCenter
        visible: source != ""
    }
}
