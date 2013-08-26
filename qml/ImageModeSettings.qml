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

Flickable {
    property Camera camera: null

    id: flick

    contentHeight: col.height
    anchors.fill: parent
    anchors.margins: 10

    Column {
        id: col
        width: parent.width
        spacing: 10

        CameraLabel {
            font.pixelSize: 36
            text: qsTr("Image settings")
        }

        ImageResolutionSettings {
            camera: flick.camera
            width: parent.width
        }

        CameraTextSwitch {
            text: qsTr("Enable face detection")
            // We have to do it that way because QML complains about a binding
            // loop for checked if we bind the checked property to the settings value.
            Component.onCompleted: checked = settings.faceDetectionEnabled
            onCheckedChanged: settings.faceDetectionEnabled = checked
            visible: !camera.quirks.hasQuirk(Quirks.NoFaceDetection)
        }

        CameraSettings {
            camera: flick.camera
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
