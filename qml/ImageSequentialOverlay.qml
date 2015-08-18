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

BaseOverlay {
    id: overlay

    policyMode: CameraResources.Image
    pressed: remainingShots > 0 || pageBeingManipulated
    inhibitDim: remainingShots > 0
    captureButtonIconSource: cameraTheme.selfTimerIconId
    canCapture: imageMode.canCapture && remainingShots == 0

    property int remainingShots: 0

    ImageMode {
        id: imageMode
        camera: cam

        enablePreview: settings.enablePreview

        onPreviewAvailable: overlay.previewAvailable(preview)

        onCanCaptureChanged: {
            // Seems canCapture can be called multiple times
            // so we explicitly check for the timer state
            if (canCapture && remainingShots > 0 && !captureTimer.running) {
                --remainingShots
                if (remainingShots > 0) {
                    countDown.value = settings.sequentialShotsInterval
                    captureTimer.start()
                }
            }

            stopCapture()
        }

        onSaved: mountProtector.unlock(platformSettings.imagePath)
    }

    Column {
        width: parent.width
        anchors.centerIn: parent

        CameraLabel {
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            styleColor: "black"
            style: Text.Outline
            font.pixelSize: cameraStyle.fontSizeLarge
            text: qsTr("Remaining shots: %1").arg(remainingShots)
            visible: remainingShots > 0
        }

        CameraLabel {
            id: countDown
            anchors.horizontalCenter: parent.horizontalCenter
            property int value: 0
            visible: remainingShots > 0
            text: value
            color: "white"
            styleColor: "black"
            style: Text.Outline
            font.pixelSize: cameraStyle.fontSizeLarge

            Timer {
                id: countDownTimer
                interval: 1000
                running: countDown.value > 0
                onTriggered: countDown.value--
                repeat: true
            }
        }
    }

    Timer {
        id: captureTimer
        interval: settings.sequentialShotsInterval * 1000
        onTriggered: {
            if (doFocus()) {
                focusAndCapture()
            } else {
                _doCaptureImage()
            }
        }
    }

    Timer {
        id: delayTimer
        interval: settings.sequentialShotsDelay * 1000
        onTriggered: {
            if (doFocus()) {
                focusAndCapture()
            } else {
                _doCaptureImage()
            }
        }
    }

    Column {
        visible: controlsVisible && !overlayCapturing && !selectedLabel.visible
        spacing: cameraStyle.spacingLarge
        anchors {
            verticalCenter: parent.verticalCenter
            left: settings.leftHandedMode ? undefined : parent.left
            leftMargin: cameraStyle.padding
            right : settings.leftHandedMode ? parent.right : undefined
            rightMargin: cameraStyle.padding
        }

        OnScreenOption {
            id: count
            visualParent: overlay
            text: qsTr("%1x").arg(settings.sequentialShotsCount)
            minimumValue: 2
            maximumValue: 20
            stepSize: 1
            value: settings.sequentialShotsCount
            onValueChanged: {
                if (pressed) {
                    settings.sequentialShotsCount = value
                }
            }

            valueIndicatorVisible: true
            valueIndicatorText: formatValue(value)
            function formatValue(value) {
                return qsTr("Capture %1 shots").arg(value)
            }
        }

        OnScreenOption {
            id: interval
            visualParent: overlay
            text: qsTr("%1s").arg(settings.sequentialShotsInterval)
            minimumValue: 0
            maximumValue: 60
            stepSize: 1
            value: settings.sequentialShotsInterval
            onValueChanged: {
                if (pressed) {
                    settings.sequentialShotsInterval = value
                }
            }

            valueIndicatorVisible: true
            valueIndicatorText: formatValue(value)
            function formatValue(value) {
                return qsTr("%1 seconds interval").arg(value)
            }
        }
    }

    CameraToolBarLabel {
        id: selectedLabel
        anchors {
            bottom: toolBar.top
            bottomMargin: cameraStyle.padding
        }
        visible: controlsVisible && !overlayCapturing && text != ""
    }

    ImageModeToolBar {
        id: toolBar
        selectedLabel: selectedLabel
        visible: controlsVisible && !overlayCapturing
    }

    ImageModeIndicators {
        id: indicators
        visible: controlsVisible && !overlayCapturing
    }

    Connections {
        target: camera.autoFocus
        onStatusChanged: {
            if (cam.autoFocus.status == AutoFocus.Success ||
                cam.autoFocus.status == AutoFocus.Fail) {
                _doCaptureImage()
            }
        }
    }

    Connections {
        target: rootWindow
        onActiveChanged: {
            if (!rootWindow.active && remainingShots > 0) {
                overlay.policyLost()
            }
        }
    }

    CaptureCancel {
        anchors.fill: parent
        enabled: remainingShots > 0
        onClicked: policyLost()
    }

    function focusAndCapture() {
        if (cam.autoFocus.cafStatus == AutoFocus.Success) {
            cam.sounds.playAutoFocusAcquiredSound()
            _doCaptureImage()
        } else {
            startAutoFocus()
        }
    }

    function doFocus() {
        return settings.focusBeforeSequentialShots && deviceFeatures().isAutoFocusSupported
    }

    function _doCaptureImage() {
        metaData.setMetaData()

        var fileName = fileNaming.imageFileName()
        if (!imageMode.capture(fileName)) {
            showError(qsTr("Failed to capture image. Please restart the camera."))
            mountProtector.unlock(platformSettings.imagePath)
            policyLost()
        } else {
            trackerStore.storeImage(fileName)
        }
    }

    function cameraError() {
        policyLost()
    }

    function policyLost() {
        remainingShots = 0
        countDown.value = 0
        delayTimer.stop()
        captureTimer.stop()
        stopCapture()
        mountProtector.unlock(platformSettings.imagePath)
    }

    function startCapture() {
        if (!imageMode.canCapture) {
            showError(qsTr("Camera is already capturing an image."))
            policyLost()
        } else if (!batteryMonitor.good) {
            showError(qsTr("Not enough battery to capture images."))
            policyLost()
        } else if (!fileSystem.available) {
            showError(qsTr("Camera cannot capture images in mass storage mode."))
            policyLost()
        } else if (!fileSystem.hasFreeSpace(platformSettings.imagePath)) {
            showError(qsTr("Not enough space to capture images."))
            policyLost()
        } else if (!mountProtector.lock(platformSettings.imagePath)) {
            showError(qsTr("Failed to lock images directory."))
            policyLost()
        } else {
            count.close()
            interval.close()
            remainingShots = settings.sequentialShotsCount
            delayTimer.start()
            countDown.value = settings.sequentialShotsDelay
        }
    }

    function startAutoFocus() {
        if (settings.focusBeforeSequentialShots && deviceFeatures().isAutoFocusSupported) {
            cam.autoFocus.startAutoFocus()
        }
    }

    function stopCapture() {
        if (deviceFeatures().isAutoFocusSupported) {
            cam.autoFocus.stopAutoFocus()
        }
    }

    function resetToolBar() {
        if (toolBar.depth() > 1) {
            toolBar.pop()
        }
    }

    function cameraDeviceChanged() {
        resetToolBar()
    }

    function applySettings() {
        var s = deviceSettings()

        camera.scene.value = s.imageSceneMode
        camera.flash.value = s.imageFlashMode
        camera.evComp.value = s.imageEvComp
        camera.whiteBalance.value = s.imageWhiteBalance
        camera.colorTone.value = s.imageColorFilter
        camera.iso.value = s.imageIso
        camera.focus.value = Focus.ContinuousNormal

        imageSettings.setImageResolution()
    }
}
