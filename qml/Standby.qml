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

@IMPORT_QT_QUICK@
import com.nokia.meego 1.0

Rectangle {
    id: standby

    property bool show: true
    property bool policyLost: false

    color: "black"
    anchors.fill: parent
    z: 2

    visible: opacity != 0.0
    opacity: show ? 1.0 : 0.0

    Behavior on opacity {
        PropertyAnimation { duration: 250 }
    }

    MouseArea {
        anchors.fill: parent
        enabled: parent.visible
    }

    Image {
        id: icon
        source: "image://theme/icon-l-camera-standby"
        anchors.centerIn: parent
    }

    Label {
        anchors.top: icon.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        text: qsTr("Resources lost")
        color: "white"
        font.pixelSize: 36
        horizontalAlignment: Text.AlignHCenter
        visible: parent.policyLost
    }
}
