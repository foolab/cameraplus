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

MouseArea {
    property alias iconSource: icon.source
    property alias color: background.color
    property alias border: background.border
    property alias radius: background.radius
    property alias iconWidth: background.width
    property alias iconHeight: background.height

    width: 64
    height: 64

    opacity: enabled ? 1.0 : 0.5

    Rectangle {
        id: background
        color: "transparent"
        width: 64
        height: 64
        anchors.centerIn: parent

        Image {
            id: icon
            anchors.centerIn: parent
            asynchronous: true
            width: parent.width * 0.75
            height: parent.height * 0.75
            visible: source != ""
        }
    }
}
