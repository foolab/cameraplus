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

Item {
    id: overlay
    property Camera cam
    property bool animationRunning: false
    property int policyMode
    property bool pressed: pageBeingManipulated
    property bool inhibitDim: false
    property bool controlsVisible: cam != null && cam.running && !modeController.busy && !animationRunning && canCapture
    property bool pageBeingManipulated: capture.pressed || zoomSlider.pressed || modeButton.pressed
    property bool canCapture: false
    property alias captureButtonIconSource: capture.iconSource
    property alias overlayCapturing: captureControl.capturing
    property alias zoomSliderVisible: zoomSlider.visible

    signal previewAvailable(string uri)

    anchors.fill: parent

    CaptureButton {
        id: capture
        visible: controlsVisible

        onExited: {
            if (mouseX <= 0 || mouseY <= 0 || mouseX > width || mouseY > height) {
                // Release outside the button:
                captureControl.canceled = true
            }
        }
    }

    ZoomCaptureButton {
        id: zoomCapture
    }

    CaptureControl {
        id: captureControl
        capturePressed: capture.pressed
        zoomPressed: zoomCapture.zoomPressed
        proximityClosed: proximitySensor.sensorClosed
        onStartCapture: overlay.startCapture()
        onCancelCapture: stopCapture()
        enable: inCaptureView
    }

    CaptureCancel {
        anchors.fill: parent
        enabled: captureControl.showCancelBanner
        onPressed: captureControl.canceled = true
    }

    ModeButton {
        id: modeButton
        anchors.horizontalCenter: capture.horizontalCenter
        anchors.top: capture.bottom
        anchors.topMargin: 20
        visible: controlsVisible && !overlayCapturing
    }

    ZoomSlider {
        id: zoomSlider
        camera: cam
        visible: controlsVisible && !captureControl.capturing
    }

}