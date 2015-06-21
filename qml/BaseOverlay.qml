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
        reversed: settings.leftHandedMode
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
        property bool opened: settings.leftHandedMode ? modeButton.x == modeButton.drag.maximumX : modeButton.x == modeButton.drag.minimumX

        x: capture.x + ((capture.width - width) / 2)
        drag.minimumX: settings.leftHandedMode ? capture.x + ((capture.width - width) / 2) : parent.width - (modeButton.width + pluginSelector.width + 20 + pluginSelector.anchors.leftMargin)
        drag.maximumX: settings.leftHandedMode ? pluginSelector.width + 20 + pluginSelector.anchors.rightMargin : capture.x + ((capture.width - width) / 2)

        property bool dragActive: drag.active
        onDragActiveChanged: {
            if (dragActive) {
                xTransition.enabled = true
            }
        }

        Behavior on x {
            id: xTransition
            PropertyAnimation { duration: 200 }
            enabled: false
        }

        anchors {
            top: capture.bottom
            topMargin: cameraStyle.padding
        }

        visible: controlsVisible && !overlayCapturing
        onVisibleChanged: modeButton.close()

        function close() {
            modeButton.x = settings.leftHandedMode ? modeButton.drag.minimumX : modeButton.drag.maximumX
        }
    }

    MouseArea {
        z: 1
        anchors.fill: parent
        enabled: modeButton.opened
        onClicked: modeButton.close()
    }

    PluginSelector {
        id: pluginSelector
        visible: inCaptureView || modeButton.opened

        z: 1
        anchors {
            left: settings.leftHandedMode ? undefined : modeButton.right
            leftMargin: parent.width - modeButton.drag.maximumX - modeButton.width
            right: settings.leftHandedMode ? modeButton.left : undefined
            rightMargin: capture.x + ((capture.width - modeButton.width) / 2)
            top: parent.top
            bottom: parent.bottom
            topMargin: cameraStyle.padding
            bottomMargin: cameraStyle.padding
        }
    }

    ZoomSlider {
        id: zoomSlider
        camera: cam
        visible: controlsVisible && !captureControl.capturing
    }

}