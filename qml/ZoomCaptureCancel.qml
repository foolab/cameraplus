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

import QtQuick 1.1
import com.nokia.meego 1.1
import CameraPlus 1.0

MouseArea {
    id: zoomCancel
    property Item zoomCapture

    signal canceled

    z: 100
    enabled: zoomCapture.zoomPressed

    onPressed: {
        zoomCancel.zoomCapture.zoomPressed = false
        zoomCancel.canceled()
    }

    Rectangle {
        anchors.top: parent.top
        opacity: parent.enabled ? 0.5 : 0
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        visible: opacity > 0
        height: label.height * 2
        width: parent.width * 0.7
        color: "black"
        border.color: "gray"
        radius: 20

        Behavior on opacity {
            PropertyAnimation { duration: 100; }
        }

        Label {
            id: label
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width
            text: qsTr("Tap anywhere to cancel")
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
