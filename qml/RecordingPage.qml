// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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

// TODO: on error ?
// TODO: resources lost?
CameraPage {
        id: page
        modesVisible: false

        Component.onCompleted: startRecording();
        Component.onDestruction: videoMode.stopRecording();

        function startRecording() {
                if (!resourcePolicy.acquired || resourcePolicy.hijacked) {
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
        }

        function stopRecording() {
                mountProtector.unlock();
                pageStack.pop(undefined, true);
        }

        policyMode: CameraResources.Recording

        controlsVisible: cam.running && videoMode.recording && !cameraMode.animationRunning && !previewAnimationRunning && !standbyWidget.visible

        orientationLock: PageOrientation.LockLandscape

        DisplayState {
                inhibitDim: true
        }

        onBatteryLow: {
                if (!checkBattery()) {
                        page.stopRecording();
                        showError(qsTr("Not enough battery to record video."));
                }
        }

        Button {
                id: recording
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                iconSource: "image://theme/icon-m-camera-video-record"
                width: 75
                height: 75
                opacity: 0.5

                onClicked: page.stopRecording();
                visible: controlsVisible
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

        VideoTorchButton {
                id: torch
                camera: cam
                visible: controlsVisible
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 20
                anchors.leftMargin: 20
                opacity: 0.5
        }

        VideoEvCompButton {
                id: evComp
                visible: controlsVisible
                anchors.top: torch.bottom
                anchors.left: parent.left
                anchors.topMargin: 10
                anchors.leftMargin: 20
        }

        Rectangle {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: 20
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
                        triggeredOnStart: true
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

                        onRunningChanged: {
                                if (!running) {
                                        duration = 0;
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
                                var secs = parseInt(duration);
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
}
