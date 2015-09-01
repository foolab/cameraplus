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
import QtCamera 1.0
import CameraPlus 1.0

Flickable {
    id: flick

    contentHeight: col.height

    anchors {
        fill: parent
        margins: cameraStyle.spacingMedium
    }

    Column {
        id: col
        width: parent.width
        spacing: cameraStyle.spacingMedium

        // Plugin settings:
        Loader {
            property Camera camera: viewfinder.camera
            width: parent.width
            source: activePlugin.settings
        }

        // Common settings:
        CameraLabel {
            font.pixelSize: cameraStyle.fontSizeLarge
            text: qsTr("Camera settings")
            width: parent.width
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
            text: qsTr("Left handed mode (requires restart)")
            checked: settings.leftHandedMode
            onCheckedChanged: settings.leftHandedMode = checked
        }

        CameraTextSwitch {
            id: useGps
            text: qsTr("Use GPS")

            checked: settings.useGps
            onCheckedChanged: settings.useGps = checked
        }

        CameraTextSwitch {
            enabled: useGps.checked
            text: qsTr("Use geotags")
            checked: settings.useGeotags
            onCheckedChanged: settings.useGeotags = checked
        }

    }
}
