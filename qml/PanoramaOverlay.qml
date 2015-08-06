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
import CameraPlus.Panorama 1.0

// TODO: disable proximity capture
// TODO: disable zoom capture
// TODO: disable zooming via rocker

BaseOverlay {
    id: overlay

    policyMode: CameraResources.Image
    pressed: processing || pageBeingManipulated
    inhibitDim: processing
    captureButtonIconSource: cameraTheme.captureButtonImageIconId
    canCapture: imageMode.canCapture && !processing
    zoomSliderVisible: false
    enableFocus: false
    enableRoi: false
    renderingEnabled: panorama.status != Panorama.Stitching

    property bool processing

    Panorama {
        id: panorama
        input: panoramaInput
    }

    PanoramaInput {
        id: panoramaInput
    }

    ViewfinderBufferHandler {
        camera: cam
        handler: panoramaInput
        enabled: panorama.status == Panorama.Tracking
    }

    ImageMode {
        id: imageMode
        camera: cam

        onCaptureEnded: stopCapture()

        enablePreview: settings.enablePreview

        onPreviewAvailable: overlay.previewAvailable(preview)

        onSaved: mountProtector.unlock(platformSettings.imagePath)
    }

    Connections {
        target: rootWindow
        onActiveChanged: {
            if (!rootWindow.active && overlay.processing) {
                overlay.policyLost()
            }
        }
    }

    function cameraError() {
        policyLost()
    }

    function policyLost() {
        // TODO: stop panorama mode
        stopCapture()
        mountProtector.unlock(platformSettings.imagePath)
    }

    function startCapture() {
        if (!batteryMonitor.good) {
            showError(qsTr("Not enough battery to capture images."))
            stopCapture()
        } else if (!fileSystem.available) {
            showError(qsTr("Camera cannot capture images in mass storage mode."))
            stopCapture()
        } else if (!fileSystem.hasFreeSpace(platformSettings.imagePath)) {
            showError(qsTr("Not enough space to capture images."))
            stopCapture()
        } else if (!mountProtector.lock(platformSettings.imagePath)) {
            showError(qsTr("Failed to lock images directory."))
            stopCapture()
        } else {
            panorama.start()
        }
    }

    function stopCapture() {
        // TODO:
    }

    function cameraDeviceChanged() {
        // Nothing
    }

    function applySettings() {
            // TODO:
        var s = deviceSettings()

        camera.scene.value = Scene.Manual
        camera.flash.value = Flash.Off
        camera.evComp.value = 0
        camera.whiteBalance.value = WhiteBalance.Auto
        camera.colorTone.value = ColorTone.Normal
        camera.iso.value = 0
        camera.focus.value = Focus.Auto

        imageSettings.setViewfinderResolution(Qt.size(640, 480))
    }

    CameraSlider {
        anchors.centerIn: parent
        minimumValue: 0
        maximumValue: 100
        stepSize: 1
        valueIndicatorVisible: false
        handleVisible: false
        enabled: false
        opacity: 0.5
        value: panorama.stitchingProgress
        visible: panorama.status == Panorama.Stitching
    }

    CameraSlider {
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        minimumValue: 0
        maximumValue: 100
        stepSize: 1
        valueIndicatorVisible: false
        handleVisible: false
        enabled: false
        opacity: 0.5
        value: panorama.frameCount
        visible: panorama.status == Panorama.Tracking
    }
}
