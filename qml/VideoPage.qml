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

        policyMode: CameraResources.Video

        controlsVisible: cam.running && !videoMode.recording && videoMode.canCapture && !cameraMode.busy && dimmer.opacity == 0.0 && !previewAnimationRunning
        orientationLock: PageOrientation.LockLandscape

        property Item settingsDialog: null

        function startRecording() {
                if (!fileSystem.available) {
                        showError(qsTr("Camera cannot record videos in mass storage mode."));
                        return;
                }

                if (!checkBattery()) {
                        showError(qsTr("Not enough battery to record video."));
                        return;
                }

                if (!checkDiskSpace()) {
                        showError(qsTr("Not enough space to record video."));
                        return;
                }

                openFileNow("RecordingPage.qml");
        }

        ZoomCaptureButton {
                id: zoomCapture
                page: page
                onReleased: page.startRecording();
        }

        ZoomCaptureCancel {
                anchors.fill: parent
                page: page
                zoomCapture: zoomCapture
        }

        CaptureButton {
                id: recording
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                iconSource: "image://theme/icon-m-camera-video-record"
                width: 75
                height: 75
                opacity: 0.5

                onClicked: startRecording();

                visible: controlsVisible && (!settings.zoomAsShutter && keys.active)
        }

        VideoMode {
                id: videoMode
                camera: cam
                onPreviewAvailable: {
                        page.setPreview(preview);
                }
        }

        Rectangle {
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
                                source: "image://theme/" + Data.videoIcon(settings.videoResolution);
                        }

                        Indicator {
                                id: wbIndicator
                                source: visible ? "image://theme/" + Data.wbIcon(settings.videoWhiteBalance) + "-screen" : ""
                                visible: settings.videoWhiteBalance != WhiteBalance.Auto
                        }

                        Indicator {
                                id: cfIndicator
                                source: "image://theme/" + Data.cfIcon(settings.videoColorFilter) + "-screen"
                                visible: settings.videoColorFilter != ColorTone.Normal
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
                VideoTorchButton {
                        camera: cam
                },
                VideoSceneButton {
                        onClicked: toolBar.push(items);
                },
                VideoEvCompButton {
                        onClicked: toolBar.push(items);
                },
                VideoWhiteBalanceButton {
                        onClicked: toolBar.push(items);
                },
                VideoColorFilterButton {
                        onClicked: toolBar.push(items);
                },
                VideoMuteButton {
                },
                ToolIcon {
                        iconSource: "image://theme/icon-m-toolbar-view-menu-white"
                        onClicked: openSettings();
                }
                ]
        }

        function openSettings() {
                if (!settingsDialog) {
                        settingsDialog = videoSettingsDialog.createObject(page);
                }

                settingsDialog.open();
        }

        Component {
                id: videoSettingsDialog

                VideoSettingsDialog { }
        }
}
