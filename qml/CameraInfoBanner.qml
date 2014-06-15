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
    property alias text: label.text
    property bool shown: false

    width: parent.width - 16
    height: label.height * 2
    y: shown ? 8 : - (height + 8)
    anchors.horizontalCenter: parent.horizontalCenter
    color: "black"
    border.color: "gray"
    radius: 20
    opacity: shown ? 1.0 : 0

    Behavior on y {
        NumberAnimation {duration: 200}
    }

    Behavior on opacity {
        NumberAnimation {duration: 200}
    }

    Timer {
        id: timer
        repeat: false
        interval: 3000
        onTriggered: hide()
    }

    MouseArea {
        anchors.fill: parent
        onClicked: parent.hide()
    }

    CameraLabel {
        id: label
        anchors.verticalCenter: parent.verticalCenter
        elide: Text.ElideRight
        maximumLineCount: 1
        x: 16
    }

    function show() {
        shown = true
        timer.restart()
    }

    function hide() {
        shown = false
        timer.stop()
    }
}
