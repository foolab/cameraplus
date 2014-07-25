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
    spacing: 10

    CameraLabel {
        font.pixelSize: 36
        text: qsTr("Sequential shots settings")
    }

    ImageResolutionSettings {
        width: parent.width
    }

    Column {
        width: parent.width

        CameraLabel {
            text: qsTr("Delay before capturing the first image (0 for no delay): %1").arg(settings.sequentialShotsDelay)
        }

        CameraSlider {
            anchors.horizontalCenter: parent.horizontalCenter
            width: (parent.width * 3) / 4
            minimumValue: 0
            maximumValue: 60
            stepSize: 1
            value: settings.sequentialShotsDelay
            onValueChanged: {
                if (pressed) {
                    settings.sequentialShotsDelay = value
                }
            }

            valueIndicatorText: formatValue(value)
            function formatValue(value) {
                return qsTr("%1 seconds").arg(value)
            }
        }
    }

    CameraTextSwitch {
        text: qsTr("Try to focus before capturing images")
        checked: settings.focusBeforeSequentialShots
        onCheckedChanged: settings.focusBeforeSequentialShots = checked
    }

    CameraTextSwitch {
        text: qsTr("Enable face detection")
        checked: settings.faceDetectionEnabled
        onCheckedChanged: settings.faceDetectionEnabled = checked
        visible: deviceFeatures().isFaceDetectionSupported
    }
}
