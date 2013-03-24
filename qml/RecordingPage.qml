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

// TODO: optional resources?

CameraPage {
        id: page
        modesVisible: false
        property bool error: false

        policyMode: CameraResources.Recording

        controlsVisible: cam.running && videoMode.recording && !error

        orientationLock: PageOrientation.LockLandscape

        function policyLost() {
                page.stopRecording();
        }

        function cameraError() {
                error = true;
                page.stopRecording();
        }

        onStatusChanged: {
                if (page.status == PageStatus.Active) {
                        startRecording();
                }
        }

        function startRecording() {
                if (!pipelineManager.acquired || pipelineManager.hijacked) {
                        showError(qsTr("Failed to acquire needed resources."));
                        pageStack.pop(undefined, true);
                        return;
                }

                metaData.setMetaData();

                if (!mountProtector.lock()) {
                        showError(qsTr("Failed to lock images directory."));
                        pageStack.pop(undefined, true);
                        return;
                }

                var file = fileNaming.videoFileName();
                var tmpFile = fileNaming.temporaryVideoFileName();

                if (!videoMode.startRecording(file, tmpFile)) {
                        showError(qsTr("Failed to record video. Please restart the camera."));
                        pageStack.pop(undefined, true);
                        mountProtector.unlock();
                }

                trackerStore.storeVideo(file);
        }

        function stopRecording() {
                videoMode.stopRecording(true);
                mountProtector.unlock();
                // Something is fishy here but if there is an error
                // and we use immediate mode then the page never gets destroyed.
                pageStack.pop(undefined, error ? false : true);
        }

        DisplayState {
                inhibitDim: true
        }

        onBatteryLow: {
                if (!checkBattery()) {
                        page.stopRecording();
                        showError(qsTr("Not enough battery to record video."));
                }
        }

        ZoomCaptureButton {
                id: zoomCapture
                page: page
                onReleased: page.stopRecording();
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

                onClicked: page.stopRecording();
                visible: controlsVisible && (!settings.zoomAsShutter && keys.active)
        }

        Connections {
                target: Qt.application
                onActiveChanged: {
                        if (!Qt.application.active) {
                                page.stopRecording();
                        }
                }
        }

        VideoMode {
                id: videoMode
                camera: cam
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

        Rectangle {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.bottomMargin: 20

                visible: controlsVisible

                color: "black"
                opacity: 0.5
                width: 100
                height: 30

                Timer {
                        id: recordingDuration

                        property int duration: 0

                        running: videoMode.recording
                        interval: 1000
                        repeat: true

                        onTriggered: {
                                duration = duration + 1;
                                if (duration == 3600) {
                                        page.stopRecording();
                                        showError(qsTr("Maximum recording time reached."));
                                }
                                else if (!checkDiskSpace()) {
                                        page.stopRecording();
                                        showError(qsTr("Not enough space to continue recording."));
                                }
                        }
                }

                Image {
                        id: recordingIcon
                        source: "image://theme/icon-m-camera-ongoing-recording"
                        width: 20
                        height: 20
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        sourceSize.width: 20
                        sourceSize.height: 20
                }

                Label {
                        function formatDuration(dur) {
                                var secs = parseInt(recordingDuration.duration);
                                var minutes = Math.floor(secs / 60);
                                var seconds = secs - (minutes * 60);

                                var date = new Date();
                                date.setSeconds(seconds);
                                date.setMinutes(minutes);
                                return Qt.formatTime(date, "mm:ss");
                        }

                        id: durationLabel
                        text: formatDuration(recordingDuration.duration);
                        anchors.left: recordingIcon.right
                        anchors.leftMargin: 5
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
                onExpandedChanged: settings.showToolBar = expanded;

                items: [
                VideoTorchButton {
                        camera: cam
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
                        onClicked: openFile("VideoSettingsPage.qml");
                }
                ]
        }
}
