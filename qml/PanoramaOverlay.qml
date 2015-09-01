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

// TODO: show preview while stitching?
// TODO: wide strip?
// TODO: metadata

BaseOverlay {
    id: overlay

    policyMode: CameraResources.Image
    pressed: processing || pageBeingManipulated
    inhibitDim: processing
    captureButtonIconSource: cameraTheme.captureButtonImageIconId
    canCapture: imageMode.canCapture && panorama.status != Panorama.Stitching
    zoomSliderVisible: false
    enableFocus: false
    enableRoi: false
    renderingEnabled: panorama.status != Panorama.Stitching
    overlayCapturing: processing
    zoomCaptureEnabled: false
    proximityCaptureEnabled: false

    property bool processing: panorama.status != Panorama.Idle
    property variant lowResolution: Qt.size(640, 480)
    property variant highResolution: Qt.size(960, 540)

    Panorama {
        id: panorama
        input: panoramaInput
        keepFrames: settings.panoramaKeepFrames
        jpegQuality: settings.panoramaJpegQuality
        onError: {
            console.log("Panorama error " + errorCode)
            switch (errorCode) {
                    case Panorama.ErrorTrackerInit:
                        showError(qsTr("Failed to initialize panorama tracker"))
                        break;
                    case Panorama.ErrorTrackerFormat:
                        showError(qsTr("Unknown camera color format"))
                        break;
                    case Panorama.ErrorTrackerConvert:
                        showError(qsTr("Failed to analyze camera frame"))
                        break;

                    case Panorama.ErrorIntermediatesDirectory:
                    case Panorama.ErrorIntermediatesConvert:
                    case Panorama.ErrorIntermediatesSave:
                        showError(qsTr("Failed to save intermediate frames"))
                        break;
                    case Panorama.ErrorStitch:
                        showError(qsTr("Failed to generate the panorama image"))
                        break;
                    case Panorama.ErrorSave:
                        showError(qsTr("Failed to save panorama image"))
                        break;
                    default:
                        showError(qsTr("Unknown error from panorama generate"))
                        break;
            }

            stop()
        }
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
    }

    CameraToolIcon {
        border {
            width: 1
            color: cameraStyle.borderColor
        }

        anchors {
            right: parent.right
            rightMargin: cameraStyle.padding
            bottom: parent.bottom
            bottomMargin: cameraStyle.padding
        }
        visible: panorama.status != Panorama.Idle
        iconSource: cameraTheme.deleteIconId
        onClicked: stopPanorama()
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
        stitchPanorama()
    }

    function startCapture() {
        if (panorama.status != Panorama.Idle) {
            // TODO: allow stitching only if we have more than X frames?
            stitchPanorama()
        } else {
            startPanorama()
        }
    }

    function stitchPanorama() {
        panorama.stitch()
    }

    function startPanorama() {
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
            var fileName = fileNaming.imageFileNameWithExtension("tif")
            panorama.start(fileName)
        }
    }

    function stopPanorama() {
        panorama.stop()
        mountProtector.unlock(platformSettings.imagePath)
    }

    function stopCapture() {
        // This is a callback needed by CaptureControln when user cancels
        // an attempt to capture. It's relevant only for images
    }

    function cameraDeviceChanged() {
        // Nothing
    }

    function applySettings() {
        var s = deviceSettings()

        camera.scene.value = Scene.Manual
        camera.flash.value = Flash.Off
        camera.evComp.value = 0
        camera.whiteBalance.value = WhiteBalance.Auto
        camera.colorTone.value = ColorTone.Normal
        camera.iso.value = 0
        camera.focus.value = Focus.Auto

        setPanoramaResolution()
    }

    Connections {
        target: settings
        onPanoramaUseHighResolutionChanged: setPanoramaResolution()
    }

    function setPanoramaResolution() {
        var res = settings.panoramaUseHighResolution ? highResolution : lowResolution

        if (!imageSettings.setViewfinderResolution(res)) {
            showError(qsTr("Failed to set resolution."))
        }
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
