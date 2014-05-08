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

MouseArea {
    id: dialog

    signal triggered

    anchors.fill: parent
    visible: opacity > 0
    enabled: visible
    opacity: 0

    Behavior on opacity {
        NumberAnimation { duration: 100 }
    }

    function open(fileName) {
        message.text = fileName
        opacity = 1
    }

    function close() {
        opacity = 0
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.8
    }

    Column {
        anchors.centerIn: parent
        spacing: 20

        CameraLabel {
            width: parent.width
            font.pixelSize: 36
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("Delete item?")
        }

        CameraLabel {
            id: message
            width: parent.width
            font.pixelSize: 32
            horizontalAlignment: Text.AlignHCenter
        }

        CameraButton {
            text: qsTr("Yes")
            onClicked: {
                dialog.triggered()
                dialog.close()
            }
        }

        CameraButton {
            text: qsTr("No")
            onClicked: {
                dialog.close()
            }
        }
    }
}
