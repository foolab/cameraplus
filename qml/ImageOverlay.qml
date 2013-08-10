// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
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
        && dimmer.opacity == 0.0 && !cameraMode.busy

    signal previewAvailable(string uri)

    anchors.fill: parent

    ImageMode {
        id: imageMode
        camera: cam

        onCaptureEnded: cam.autoFocus.stopAutoFocus()

        onPreviewAvailable: overlay.previewAvailable(preview)

        onSaved: mountProtector.unlock(fileNaming.imagePath)
    }

    ZoomSlider {
        id: zoomSlider
        camera: cam
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
        visible: controlsVisible
    }

    ModeButton {
        id: modeButton
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottomMargin: 20
        visible: controlsVisible
    }

    CaptureButton {
        id: capture
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        iconSource: cameraTheme.captureButtonImageIconId
        width: 75
        height: 75
        opacity: 0.5
        onClicked: captureImage()
        visible: controlsVisible && (!settings.zoomAsShutter && keys.active)

        onExited: {
            if (mouseX <= 0 || mouseY <= 0 || mouseX > width || mouseY > height) {
                // Release outside the button:
                cam.autoFocus.stopAutoFocus()
            }
        }
    }

    Timer {
        id: autoFocusTimer
        interval: 200
        running: capture.pressed || zoomCapture.zoomPressed
        repeat: false
        onTriggered: {
            if (cam.autoFocus.cafStatus != AutoFocus.Success) {
                cam.autoFocus.startAutoFocus()
            }
        }
    }

    ZoomCaptureButton {
        id: zoomCapture
        onReleased: parent.captureImage()
    }

    ZoomCaptureCancel {
        anchors.fill: parent
        zoomCapture: zoomCapture
        onCanceled: {
            if (!autoFocusTimer.running) {
                cam.autoFocus.stopAutoFocus()
            }
        }
    }

    CameraToolBar {
        id: toolBar
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        opacity: 0.5
        targetWidth: parent.width - (anchors.leftMargin * 2) - (66 * 1.5)
        visible: controlsVisible
        expanded: settings.showToolBar
        onExpandedChanged: settings.showToolBar = expanded
        tools: CameraToolBarTools {
            FlashButton {
                onClicked: toolBar.push(tools)
            }

            ImageSceneButton {
                onClicked: toolBar.push(tools)
            }

            ImageEvCompButton {
                onClicked: toolBar.push(tools)
            }

            ImageWhiteBalanceButton {
                onClicked: toolBar.push(tools)
            }

            ImageColorFilterButton {
                onClicked: toolBar.push(tools)
            }

            ImageIsoButton {
                onClicked: toolBar.push(tools)
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
        height: col.height
        color: "black"
        border.color: "gray"
        radius: 20
        opacity: 0.5
        visible: controlsVisible

        Column {
            id: col
            width: parent.width
            spacing: 5

            Indicator {
                id: flashIndicator
                visible: !toolBar.expanded
                source: cameraTheme.flashIcon(settings.imageFlashMode)
            }

            CameraLabel {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                anchors.topMargin: 5
                anchors.bottomMargin: 5
                property string mp: imageSettings.currentResolutionMegapixel == "" ? "?" : imageSettings.currentResolutionMegapixel
                text: qsTr("%1M").arg(mp)
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Indicator {
                id: wbIndicator
                source: visible ? cameraTheme.whiteBalanceIcon(settings.imageWhiteBalance) : ""
                visible: settings.imageWhiteBalance != WhiteBalance.Auto && !toolBar.expanded
            }

            Indicator {
                id: cfIndicator
                source: visible ? cameraTheme.colorFilterIcon(settings.imageColorFilter) : ""
                visible: settings.imageColorFilter != ColorTone.Normal && !toolBar.expanded
            }

            Indicator {
                id: isoIndicator
                visible: settings.imageIso != 0 && !toolBar.expanded
                source: visible ? cameraTheme.isoIcon(settings.imageIso) : ""
            }

            Indicator {
                id: sceneIndicator
                visible: settings.imageSceneMode != Scene.Auto && !toolBar.expanded
                source: visible ? cameraTheme.imageSceneModeIcon(settings.imageSceneMode) : ""
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
        mountProtector.unlock(fileNaming.imagePath)
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
            cam.autoFocus.stopAutoFocus()
        } else if (!checkBattery()) {
            showError(qsTr("Not enough battery to capture images."))
            cam.autoFocus.stopAutoFocus()
        } else if (!fileSystem.available) {
            showError(qsTr("Camera cannot capture images in mass storage mode."))
            cam.autoFocus.stopAutoFocus()
        } else if (!fileSystem.hasFreeSpace(fileNaming.imagePath)) {
            showError(qsTr("Not enough space to capture images."))
            cam.autoFocus.stopAutoFocus()
        } else if (!mountProtector.lock(fileNaming.imagePath)) {
            showError(qsTr("Failed to lock images directory."))
            cam.autoFocus.stopAutoFocus()
        } else {
            metaData.setMetaData()

            var fileName = fileNaming.imageFileName()
            if (!imageMode.capture(fileName)) {
                showError(qsTr("Failed to capture image. Please restart the camera."))
                mountProtector.unlock(fileNaming.imagePath)
                cam.autoFocus.stopAutoFocus()
            } else {
                trackerStore.storeImage(fileName)
            }
        }
    }

}
