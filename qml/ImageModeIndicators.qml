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

Rectangle {
    id: indicators
    anchors {
        top: parent.top
        topMargin: 20
        left: parent.left
        leftMargin: 20
    }

    width: 48
    height: col.height + radius * 2
    color: "black"
    border.color: "gray"
    radius: 20
    opacity: 0.5
    visible: controlsVisible && !captureControl.capturing

    Column {
        id: col
        width: parent.width
        spacing: 5
        anchors.centerIn: parent

        Indicator {
            id: flashIndicator
            visible: deviceFeatures().numberOfFlashModes > 1
            source: cameraTheme.flashIcon(deviceSettings().imageFlashMode)
        }

        Indicator {
            id: wbIndicator
            source: visible ? cameraTheme.whiteBalanceIcon(deviceSettings().imageWhiteBalance) : ""
            visible: deviceSettings().imageWhiteBalance != WhiteBalance.Auto
        }

        Indicator {
            id: cfIndicator
            source: visible ? cameraTheme.colorFilterIcon(deviceSettings().imageColorFilter) : ""
            visible: deviceSettings().imageColorFilter != ColorTone.Normal
        }

        Indicator {
            id: isoIndicator
            visible: deviceSettings().imageIso != 0
            source: visible ? cameraTheme.isoIcon(deviceSettings().imageIso) : ""
        }

        Indicator {
            id: sceneIndicator
            visible: deviceSettings().imageSceneMode != Scene.Auto
            source: visible ? cameraTheme.imageSceneModeIcon(deviceSettings().imageSceneMode) : ""
        }

        Indicator {
            id: gpsIndicator
            visible: settings.useGps
            source: cameraTheme.gpsIndicatorIcon

            PropertyAnimation on opacity  {
                easing.type: Easing.OutSine
                loops: Animation.Infinite
                from: 0.2
                to: 1.0
                duration: 1000
                running: settings.useGps && !positionSource.position.horizontalAccuracyValid && viewfinder.camera.running
                alwaysRunToEnd: true
            }
        }

        Indicator {
            id: faceDetectionIndicator
            visible: settings.faceDetectionEnabled
            source: cameraTheme.faceDetectionIndicatorIcon
        }
    }
}
