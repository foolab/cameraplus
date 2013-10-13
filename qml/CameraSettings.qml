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
import QtCamera 1.0

Column {
    property Camera camera

    id: col
    spacing: 10
    width: parent.width

    CameraLabel {
        font.pixelSize: 36
        text: qsTr("Camera settings")
        width: parent.width
    }

    SectionHeader {
        text: qsTr("Camera")
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        enabled: camera != null && camera.running && camera.idle

        CameraButton {
            text: qsTr("Back (Primary)");
            checkable: true
            checked: settings.device == 0
            onClicked: settings.device = 0
        }

        CameraButton {
            text: qsTr("Front (Secondary)");
            checkable: true
            checked: settings.device == 1
            onClicked: settings.device = 1
        }
    }

    CameraTextSwitch {
        text: qsTr("Show grid lines")

        checked: settings.gridEnabled
        onCheckedChanged: settings.gridEnabled = checked
    }

    SectionHeader {
        text: qsTr("Creator name")
    }

    CameraTextField {
        placeholderText: qsTr("Name or copyright")
        width: parent.width
        text: settings.creatorName
        onTextChanged: settings.creatorName = text
    }

    CameraTextSwitch {
        text: qsTr("Use zoom keys for capture")
        checked: settings.zoomAsShutter
        onCheckedChanged: settings.zoomAsShutter = checked
    }

    CameraTextSwitch {
        text: qsTr("Use proximity sensor for capture")

        checked: settings.proximityAsShutter
        onCheckedChanged: settings.proximityAsShutter = checked
    }

    CameraTextSwitch {
        text: qsTr("Enable camera sounds")

        checked: settings.soundEnabled
        onCheckedChanged: settings.soundEnabled = checked
    }

    CameraTextSwitch {
        text: qsTr("Preview images and videos after capturing")
        checked: settings.enablePreview
        onCheckedChanged: settings.enablePreview = checked
    }

    CameraTextSwitch {
        text: qsTr("Enable night mode (Viewfinder dimming)")
        checked: settings.nightMode
        onCheckedChanged: settings.nightMode = checked
    }

    CameraTextSwitch {
        id: useGps
        text: qsTr("Use GPS")

        checked: settings.useGps
        onCheckedChanged: settings.useGps = checked
    }

    CameraTextSwitch {
        // TODO: transition when hiding/showing and we should scroll a bit to show it
        visible: useGps.checked

        text: qsTr("Use geotags")
        checked: settings.useGeotags
        onCheckedChanged: settings.useGeotags = checked
    }
}
