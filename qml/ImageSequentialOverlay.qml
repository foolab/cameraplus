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
    property int policyMode: CameraResources.Image
    property bool pressed: capture.pressed || zoomSlider.pressed || modeButton.pressed || remainingShots > 0
    property bool controlsVisible: imageMode.canCapture && cam.running && !animationRunning
        && !modeController.busy && remainingShots == 0
    property bool inhibitDim: remainingShots > 0

    property int remainingShots: 0

    signal previewAvailable(string uri)

    anchors.fill: parent

    ImageMode {
        id: imageMode
        camera: cam

        onCaptureEnded: stopAutoFocus()

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
        }

        onSaved: mountProtector.unlock(platformSettings.imagePath)
    }

    ZoomSlider {
        id: zoomSlider
        camera: cam
        visible: controlsVisible && !captureControl.capturing
    }

    Column {
        width: parent.width
        anchors.centerIn: parent

        CameraLabel {
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            styleColor: "black"
            style: Text.Outline
            font.pixelSize: 36
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
            font.pixelSize: 36

            Timer {
                id: countDownTimer
                interval: 1000
                running: countDown.value > 0
                onTriggered: countDown.value--
                repeat: true
            }
        }
    }

    ModeButton {
        id: modeButton
        anchors.horizontalCenter: capture.horizontalCenter
        anchors.top: capture.bottom
        anchors.topMargin: 20
        visible: controlsVisible && !captureControl.capturing
    }

    CaptureButton {
        id: capture
        iconSource: cameraTheme.selfTimerIconId
        visible: controlsVisible

        onExited: {
            if (mouseX <= 0 || mouseY <= 0 || mouseX > width || mouseY > height) {
                // Release outside the button:
                captureControl.canceled = true
            }
        }
    }

    Timer {
        id: autoFocusTimer
        interval: 200
        running: captureControl.state == "capturing"
        repeat: false
        onTriggered: {
            if (cam.autoFocus.cafStatus != AutoFocus.Success) {
                startAutoFocus()
            } else {
                cam.sounds.playAutoFocusAcquiredSound()
            }
        }
    }

    Timer {
        id: captureTimer
        interval: settings.sequentialShotsInterval * 1000
        onTriggered: _doCaptureImage()
    }

    Timer {
        id: delayTimer
        interval: settings.sequentialShotsDelay * 1000
        onTriggered: _doCaptureImage()
    }

    ZoomCaptureButton {
        id: zoomCapture
    }

    CaptureControl {
        id: captureControl
        capturePressed: capture.pressed
        zoomPressed: zoomCapture.zoomPressed
        proximityClosed: proximitySensor.sensorClosed
        onStartCapture: captureImage()
        onCancelCapture: stopAutoFocus()
        enable: inCaptureView
    }

    CaptureCancel {
        anchors.fill: parent
        enabled: captureControl.showCancelBanner
        onPressed: captureControl.canceled = true
    }

    CameraSlider {
        id: count

        anchors {
            top: zoomSlider.bottom
            topMargin: 40
            horizontalCenter: parent.horizontalCenter
        }

        visible: controlsVisible && !captureControl.capturing && !selectedLabel.visible
        width: (parent.width * 3) / 4
        opacity: 0.8
        minimumValue: 2
        maximumValue: 60
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
            return qsTr("%1 shots").arg(value)
        }
    }

    CameraSlider {
        id: interval

        anchors {
            bottom: toolBar.top
            horizontalCenter: parent.horizontalCenter
        }

        visible: controlsVisible && !captureControl.capturing && !selectedLabel.visible
        width: (parent.width * 3) / 4
        opacity: 0.8
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

    CameraToolBarLabel {
        id: selectedLabel
        anchors.bottom: toolBar.top
        anchors.bottomMargin: 20
        visible: controlsVisible && !captureControl.capturing && text != ""
    }

    ImageModeToolBar {
        id: toolBar
        selectedLabel: selectedLabel
        visible: controlsVisible && !captureControl.capturing
    }

    ImageModeIndicators {
        id: indicators
        visible: controlsVisible && !captureControl.capturing
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

    function _doCaptureImage() {
        metaData.setMetaData()

        var fileName = fileNaming.imageFileName()
        if (!imageMode.capture(fileName)) {
            showError(qsTr("Failed to capture image. Please restart the camera."))
            mountProtector.unlock(platformSettings.imagePath)
            stopAutoFocus()
            remainingShots = 0
            countDown.value = 0
            delayTimer.stop()
            captureTimer.stop()
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
        stopAutoFocus()
        mountProtector.unlock(platformSettings.imagePath)
    }

    function captureImage() {
        if (!imageMode.canCapture) {
            showError(qsTr("Camera is already capturing an image."))
            stopAutoFocus()
        } else if (!batteryMonitor.good) {
            showError(qsTr("Not enough battery to capture images."))
            stopAutoFocus()
        } else if (!fileSystem.available) {
            showError(qsTr("Camera cannot capture images in mass storage mode."))
            stopAutoFocus()
        } else if (!fileSystem.hasFreeSpace(platformSettings.imagePath)) {
            showError(qsTr("Not enough space to capture images."))
            stopAutoFocus()
        } else if (!mountProtector.lock(platformSettings.imagePath)) {
            showError(qsTr("Failed to lock images directory."))
            stopAutoFocus()
        } else {
            remainingShots = settings.sequentialShotsCount
            delayTimer.start()
            countDown.value = settings.sequentialShotsDelay
        }
    }

    function startAutoFocus() {
        if (deviceFeatures().isAutoFocusSupported) {
            cam.autoFocus.startAutoFocus()
        }
    }

    function stopAutoFocus() {
        if (deviceFeatures().isAutoFocusSupported) {
            if (!autoFocusTimer.running) {
                cam.autoFocus.stopAutoFocus()
            }
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
