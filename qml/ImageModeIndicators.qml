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

Row {
    id: indicators

    anchors {
        top: parent.top
        topMargin: cameraStyle.padding
        left: settings.leftHandedMode ? undefined: parent.left
        leftMargin: cameraStyle.padding
        right: settings.leftHandedMode ? parent.right : undefined
        rightMargin: cameraStyle.padding
    }

    height: cameraStyle.indicatorArrayHeight
    spacing: cameraStyle.spacingSmall
    visible: controlsVisible && !captureControl.capturing

    CameraLabel {
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("%1M").arg(imageMode.currentResolution.megaPixels)
        font.bold: true
        height: parent.height
    }

    Image {
        id: gpsIndicator
        height: parent.height
        width: height
        visible: settings.useGps
        asynchronous: true
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

    Image {
        id: faceDetectionIndicator
        height: parent.height
        width: height
        asynchronous: true
        visible: deviceFeatures().isFaceDetectionSupported ? settings.faceDetectionEnabled : false
        source: cameraTheme.faceDetectionIndicatorIcon
    }
}
