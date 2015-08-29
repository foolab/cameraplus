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

Column {
    width: parent.width
    spacing: cameraStyle.spacingMedium

    CameraLabel {
        font.pixelSize: cameraStyle.fontSizeLarge
        text: qsTr("Panorama settings")
    }

    CameraTextSwitch {
        text: qsTr("Keep intermediate frames")
        checked: settings.panoramaKeepFrames
        onCheckedChanged: settings.panoramaKeepFrames = checked
    }

    CameraTextSwitch {
        text: qsTr("Higher resolution (Slower)")
        checked: settings.panoramaUseHighResolution
        onCheckedChanged: settings.panoramaUseHighResolution = checked
    }

    CameraLabel {
        text: qsTr("Final JPEG quality")
        horizontalAlignment: Text.AlignLeft
    }

    CameraSlider {
        property bool _completed: false
        anchors.horizontalCenter: parent.horizontalCenter
        width: (parent.width * 3) / 4
        minimumValue: 1
        maximumValue: 100
        stepSize: 1
        Component.onCompleted: { _completed = true; value = settings.panoramaJpegQuality }
        onValueChanged: {
            if (_completed) {
                settings.panoramaJpegQuality = value
            }
        }
    }
}
