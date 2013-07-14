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

import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0
import CameraPlus 1.0
import "data.js" as Data

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
        onPreviewAvailable: {
            overlay.previewAvailable(preview)
            cam.autoFocus.stopAutoFocus()
        }

        onSaved: mountProtector.unlock()
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
        iconSource: "image://theme/icon-m-camera-shutter"
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
        items: [
            FlashButton {
                onClicked: toolBar.push(items)
            },
            ImageSceneButton {
                onClicked: toolBar.push(items)
            },
            ImageEvCompButton {
                onClicked: toolBar.push(items)
            },
            ImageWhiteBalanceButton {
                onClicked: toolBar.push(items)
            },
            ImageColorFilterButton {
                onClicked: toolBar.push(items)
            },
            ImageIsoButton {
                onClicked: toolBar.push(items)
            }
        ]
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
                id: resolutionIndicator
                source: "image://theme/" + Data.imageIcon(settings.imageAspectRatio, settings.imageResolution)
            }

            Indicator {
                id: wbIndicator
                source: visible ? "image://theme/" + Data.wbIcon(settings.imageWhiteBalance) + "-screen" : ""
                visible: settings.imageWhiteBalance != WhiteBalance.Auto
            }

            Indicator {
                id: cfIndicator
                source: "image://theme/" + Data.cfIcon(settings.imageColorFilter) + "-screen"
                visible: settings.imageColorFilter != ColorTone.Normal
            }

            Indicator {
                id: isoIndicator
                visible: settings.imageIso != 0
                source: "image://theme/" + Data.isoIcon(settings.imageIso)
            }

            Indicator {
                id: gpsIndicator
                visible: settings.useGps
                source: "image://theme/icon-m-camera-location"

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
                source: "image://theme/icon-m-camera-face-detection-screen"
            }

        }
    }

    function cameraError() {
        mountProtector.unlock()
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
        } else if (!checkBattery()) {
            showError(qsTr("Not enough battery to capture images."))
        } else if (!fileSystem.available) {
            showError(qsTr("Camera cannot capture images in mass storage mode."))
        } else if (!checkDiskSpace()) {
            showError(qsTr("Not enough space to capture images."))
        } else if (!mountProtector.lock()) {
            showError(qsTr("Failed to lock images directory."))
        } else {
            metaData.setMetaData()

            var fileName = fileNaming.imageFileName()
            if (!imageMode.capture(fileName)) {
                showError(qsTr("Failed to capture image. Please restart the camera."))
                mountProtector.unlock()
            } else {
                trackerStore.storeImage(fileName)
            }
        }
    }

}
