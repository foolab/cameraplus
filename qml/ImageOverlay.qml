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
    property bool pressed: capture.pressed || zoomSlider.pressed || modeButton.pressed
    property bool controlsVisible: imageMode.canCapture && cam.running && !animationRunning
        && !modeController.busy
    property bool inhibitDim: false

    signal previewAvailable(string uri)

    anchors.fill: parent

    ImageMode {
        id: imageMode
        camera: cam

        onCaptureEnded: stopAutoFocus()

        enablePreview: settings.enablePreview

        onPreviewAvailable: overlay.previewAvailable(preview)

        onSaved: mountProtector.unlock(platformSettings.imagePath)
    }

    ZoomSlider {
        id: zoomSlider
        camera: cam
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
        visible: controlsVisible && !captureControl.capturing
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
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        iconSource: cameraTheme.captureButtonImageIconId
        width: 100
        height: 100
        opacity: 0.5
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
        onStartCapture: captureImage()
        onCancelCapture: stopAutoFocus()
        enable: inCaptureView
    }

    CaptureCancel {
        anchors.fill: parent
        enabled: captureControl.showCancelBanner
        onPressed: captureControl.canceled = true
    }

    CameraToolBarLabel {
        id: selectedLabel
        anchors.bottom: toolBar.top
        anchors.bottomMargin: 20
        visible: controlsVisible && !captureControl.capturing
    }

    CameraToolBar {
        id: toolBar
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        opacity: 0.5
        targetWidth: parent.width - (anchors.leftMargin * 2)
        visible: controlsVisible && !captureControl.capturing
        expanded: settings.showToolBar
        onExpandedChanged: settings.showToolBar = expanded
        tools: CameraToolBarTools {
            CameraToolIcon {
                iconSource: cameraTheme.flashIcon(deviceSettings().imageFlashMode)
                onClicked: toolBar.push(Qt.resolvedUrl("FlashButton.qml"), {"selectedLabel": selectedLabel})
                visible: overlay.cam ? !overlay.cam.quirks.hasQuirk(Quirks.NoFlash) : false
            }

            CameraToolIcon {
                iconSource: cameraTheme.imageSceneModeIcon(deviceSettings().imageSceneMode)
                onClicked: toolBar.push(Qt.resolvedUrl("ImageSceneButton.qml"), {"selectedLabel": selectedLabel})
            }

            CameraToolIcon {
                iconSource: deviceSettings().imageEvComp == 0 ? cameraTheme.cameraManualExposureIconId : ""
                onClicked: toolBar.push(Qt.resolvedUrl("ImageEvCompButton.qml"))

                CameraLabel {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    visible: deviceSettings().imageEvComp != 0
                    text: deviceSettings().imageEvComp == 0 ? "" : deviceSettings().imageEvComp.toFixed(1)
                }
            }

            CameraToolIcon {
                iconSource: cameraTheme.whiteBalanceIcon(deviceSettings().imageWhiteBalance)
                onClicked: toolBar.push(Qt.resolvedUrl("ImageWhiteBalanceButton.qml"), {"selectedLabel": selectedLabel})
            }

            CameraToolIcon {
                iconSource: cameraTheme.colorFilterIcon(deviceSettings().imageColorFilter)
                onClicked: toolBar.push(Qt.resolvedUrl("ImageColorFilterButton.qml"), {"selectedLabel": selectedLabel})
            }

            CameraToolIcon {
                iconSource: cameraTheme.isoIcon(deviceSettings().imageIso)
                onClicked: toolBar.push(Qt.resolvedUrl("ImageIsoButton.qml"))
            }

            DeviceSelector {
                enabled: camera.idle
            }
        }
    }

    Rectangle {
        id: indicators
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
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
                visible: !overlay.cam.quirks.hasQuirk(Quirks.NoFlash)
                source: cameraTheme.flashIcon(deviceSettings().imageFlashMode)
            }

            CameraLabel {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                anchors.topMargin: 5
                anchors.bottomMargin: 5
                text: imageSettings.currentResolution ? qsTr("%1M").arg(imageSettings.currentResolution.megaPixels) : qsTr("?M")
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
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
                    running: settings.useGps && !positionSource.position.longitudeValid
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

    function cameraError() {
        mountProtector.unlock(platformSettings.imagePath)
    }

    function policyLost() {
        // Nothing
    }

    function batteryLow() {
        // Nothing
    }

    function captureImage() {
        if (!imageMode.canCapture) {
            showError(qsTr("Camera is already capturing an image."))
            stopAutoFocus()
        } else if (!checkBattery()) {
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
            metaData.setMetaData()

            var fileName = fileNaming.imageFileName()
            if (!imageMode.capture(fileName)) {
                showError(qsTr("Failed to capture image. Please restart the camera."))
                mountProtector.unlock(platformSettings.imagePath)
                stopAutoFocus()
            } else {
                trackerStore.storeImage(fileName)
            }
        }
    }

    function startAutoFocus() {
        if (!overlay.cam.quirks.hasQuirk(Quirks.NoAutoFocus)) {
            cam.autoFocus.startAutoFocus()
        }
    }

    function stopAutoFocus() {
        if (!overlay.cam.quirks.hasQuirk(Quirks.NoAutoFocus)) {
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
}
