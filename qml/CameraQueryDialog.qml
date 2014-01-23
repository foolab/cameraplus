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

    property alias titleText: title.text
    property alias acceptButtonText: acceptButton.text
    property alias rejectButtonText: rejectButton.text
    property alias messageText: message.text

    signal accepted
    signal rejected

    anchors.fill: parent
    visible: opacity > 0
    enabled: visible
    opacity: 0

    Behavior on opacity {
        NumberAnimation { duration: 100 }
    }

    function open() {
        opacity = 1
    }

    function close() {
        opacity = 0
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.7
    }

    Column {
        anchors.centerIn: parent
        spacing: 20

        CameraLabel {
            id: title
            width: parent.width
            font.pixelSize: 36
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }

        CameraLabel {
            id: message
            width: parent.width
            font.pixelSize: 32
            horizontalAlignment: Text.AlignHCenter
        }

        CameraButton {
            id: acceptButton
            onClicked: {
                dialog.accepted()
                dialog.close()
            }
        }

        CameraButton {
            id: rejectButton
            onClicked: {
                dialog.rejected()
                dialog.close()
            }
        }
    }
}
