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

CameraToolIcon {
    id: button
    property bool checked: false

    Rectangle {
        property CameraButtonStyle platformStyle: CameraButtonStyle {}

        anchors.centerIn: parent
        width: parent.width * 0.8
        height: parent.height * 0.8
        color: platformStyle.pressedColor
        opacity: checked ? 0.5 : 0.0
        visible: opacity > 0.0
        z: -1
        radius: width / 4
        border.color: "steelblue"
        border.width: 1
        smooth: true
        Behavior on opacity {
            PropertyAnimation {
                duration: 20
                easing.type: Easing.OutQuad
            }
        }
    }
}
