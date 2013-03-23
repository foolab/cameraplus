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

CameraPage {
        id: page

        policyMode: CameraResources.Image
        controlsVisible: imageMode.canCapture && !cameraMode.busy && dimmer.opacity == 0.0 && !previewAnimationRunning && cam.running

        orientationLock: PageOrientation.LockLandscape

        property Item settingsDialog: null

        function cameraError() {
                mountProtector.unlock();
        }

        function captureImage() {
                if (!imageMode.canCapture) {
                        showError(qsTr("Camera is already capturing an image."));
                        return;
                }

                if (!checkBattery()) {
                        showError(qsTr("Not enough battery to capture images."));
                        return;
                }

                if (!fileSystem.available) {
                        showError(qsTr("Camera cannot capture images in mass storage mode."));
                        return;
                }

                if (!checkDiskSpace()) {
                        showError(qsTr("Not enough space to capture images."));
                        return;
                }

                if (!mountProtector.lock()) {
                        showError(qsTr("Failed to lock images directory."));
                        return;
                }

                metaData.setMetaData();

                var fileName = fileNaming.imageFileName();
                if (!imageMode.capture(fileName)) {
                        showError(qsTr("Failed to capture image. Please restart the camera."));
                        return;
                }

                trackerStore.storeImage(fileName);
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
                onClicked: captureImage();
                visible: controlsVisible && (!settings.zoomAsShutter && keys.active)

                onExited: {
                        if (mouseX <= 0 || mouseY <= 0 || mouseX > width || mouseY > height) {
                                // Release outside the button:
                                cam.autoFocus.stopAutoFocus();
                        }
                }
        }

        ZoomCaptureButton {
                id: zoomCapture
                page: page
                onReleased: page.captureImage();
        }

        MouseArea {
                anchors.fill: parent
                enabled: zoomCapture.zoomPressed
                z: 100
                onPressed: {
                        zoomCapture.zoomPressed = false;
                        if (!autoFocusTimer.running) {
                                cam.autoFocus.stopAutoFocus();
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
                                cam.autoFocus.startAutoFocus();
                        }
                }
        }

        ImageMode {
                id: imageMode
                camera: cam
                onPreviewAvailable: {
                        page.setPreview(preview);
                        cam.autoFocus.stopAutoFocus();
                }

                onSaved: mountProtector.unlock();
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
                                source: "image://theme/" + Data.imageIcon(settings.imageAspectRatio, settings.imageResolution);
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
                                source: "image://theme/" + Data.isoIcon(settings.imageIso);
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

        Button {
                id: cameraRoll
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.topMargin: 20
                anchors.rightMargin: 20
                width: 56
                height: 56

                opacity: 0.5
                iconSource: "image://theme/icon-m-camera-roll"
                onClicked: openFile("PostCapturePage.qml");
                visible: controlsVisible
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
                onExpandedChanged: settings.showToolBar = expanded;

                items: [
                FlashButton {
                        onClicked: toolBar.push(items);
                },
                ImageSceneButton {
                        onClicked: toolBar.push(items);
                },
                ImageEvCompButton {
                        onClicked: toolBar.push(items);
                },
                ImageWhiteBalanceButton {
                        onClicked: toolBar.push(items);
                },
                ImageColorFilterButton {
                        onClicked: toolBar.push(items);
                },
                ImageIsoButton {
                        onClicked: toolBar.push(items);
                },
                ToolIcon {
                        iconSource: "image://theme/icon-m-toolbar-view-menu-white"
                        onClicked: openSettings();
                }
                ]
        }

        function openSettings() {
                var roiEnabled = cam.roi.enabled;
                cam.roi.enabled = false;

                if (!settingsDialog) {
                        settingsDialog = imageSettingsDialog.createObject(page);
                }

                settingsDialog.open();

                cam.roi.enabled = roiEnabled;
        }

        Component {
                id: imageSettingsDialog

                ImageSettingsDialog { }
        }
}
