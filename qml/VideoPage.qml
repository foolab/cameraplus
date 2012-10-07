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

CameraPage {
        id: page

        policyMode: CameraResources.Video

        controlsVisible: videoControlsVisible && !videoMode.recording
        property bool videoControlsVisible: recording.visible && cam.running && !standbyWidget.visible

        orientationLock: PageOrientation.LockLandscape

        DisplayState {
                inhibitDim: videoMode.recording
        }

        onBatteryLow: {
                if (!videoMode.recording) {
                        return;
                }

                if (!checkBattery()) {
                        videoMode.stopRecording();
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

                onClicked: buttonClicked();

                function buttonClicked() {
                        if (!fileSystem.available) {
                                showError(qsTr("Camera cannot record videos in mass storage mode."));
                                return;
                        }

                        // We only toggle the mode to video recording so
                        // policy can acquire the needed resources

                        if (policyMode == CameraResources.Video) {
                                if (!checkBattery()) {
                                        showError(qsTr("Not enough battery to record video."));
                                        return;
                                }

                                if (!checkDiskSpace()) {
                                        showError(qsTr("Not enough space to record video."));
                                        return;
                                }

                                policyMode = CameraResources.Recording;
                        }
                        else if (videoMode.recording) {
                                // We just ask to stop video.
                                videoMode.stopRecording();
                        }
                }

                Connections {
                        target: videoMode
                        onRecordingChanged: {
                                if (!videoMode.recording) {
                                        policyMode = CameraResources.Video;
                                }
                        }
                }

                Connections {
                        target: resourcePolicy
                        onAcquiredChanged: {
                                if (resourcePolicy.acquired && policyMode == CameraResources.Recording) {
                                        metaData.setMetaData();
                                        if (!videoMode.startRecording(fileNaming.videoFileName())) {
                                                showError(qsTr("Failed to record video. Please restart the camera."));
                                                policyMode = CameraResources.Video
}
                                }
                        }
                }

                visible: (videoMode.recording || videoMode.canCapture) && !cameraMode.animationRunning && !previewAnimationRunning && !standbyWidget.visible
        }

        Connections {
                target: Qt.application
                onActiveChanged: {
                        if (!Qt.application.active && videoMode.recording) {
                                videoMode.stopRecording();
                        }
                }
        }

        VideoMode {
                id: videoMode
                camera: cam
                onPreviewAvailable: {
                        if (!standbyWidget.visible) {
                                page.setPreview(preview);
                        }
                }
        }

        VideoTorchButton {
                id: torch
                visible: videoControlsVisible
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 20
                anchors.leftMargin: 20
                opacity: 0.5
        }

        VideoSceneButton {
                id: scene
                visible: controlsVisible
                anchors.top: torch.bottom
                anchors.left: parent.left
                anchors.topMargin: 10
                anchors.leftMargin: 20
        }

        EvCompButton {
                id: evComp
                visible: videoControlsVisible
                anchors.top: scene.bottom
                anchors.left: parent.left
                anchors.topMargin: 10
                anchors.leftMargin: 20
        }

        MouseArea {
                id: indicators
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 20
                width: 48
                height: col.height
                onClicked: openFile("VideoSettingsPage.qml");
                visible: controlsVisible

                BorderImage {
                        id: image
                        anchors.fill: parent
                        smooth: true
                        source: indicators.pressed ? "image://theme/meegotouch-camera-settings-indicators-background-pressed" : "image://theme/meegotouch-camera-settings-indicators-background"
                }

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
                                source: "image://theme/" + Data.wbIcon(settings.videoWhiteBalance) + "-screen"
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
                visible: controlsVisible && !videoMode.recording
        }

        Rectangle {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: 20
                anchors.bottomMargin: 20

                visible: videoMode.recording

                color: "black"
                opacity: 0.5
                width: 100
                height: 30

                Timer {
                        id: recordingDuration

                        property int duration: 0

                        running: cam.running && videoMode.recording
                        triggeredOnStart: true
                        interval: 1000
                        repeat: true

                        onTriggered: {
                                duration = duration + 1;
                                if (duration == 3600) {
                                        videoMode.stopRecording();
                                        showError(qsTr("Maximum recording time reached."));
                                }
                                else if (!checkDiskSpace()) {
                                        videoMode.stopRecording();
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
                                var secs = parseInt(dur);
                                var mins = Math.floor(secs / 60);
                                var seconds = secs - (mins * 60);

                                if (mins < 10) {
                                        mins = "0" + mins;
                                }

                                if (seconds < 10) {
                                        seconds = "0" + seconds;
                                }

                                return mins + ":" + seconds;
                        }

                        id: durationLabel
                        text: formatDuration(recordingDuration.duration);
                        anchors.left: recordingIcon.right
                        anchors.leftMargin: 5
                }
        }
}
