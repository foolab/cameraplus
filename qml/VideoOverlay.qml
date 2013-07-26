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
import "data.js" as Data

Item {
    id: overlay
    property bool recording: false

    property Camera cam
    property bool animationRunning: false
    property int policyMode: recording == true ? CameraResources.Recording : CameraResources.Video
    property bool controlsVisible: !animationRunning && cam != null && cam.running
        && dimmer.opacity == 0.0 && !cameraMode.busy
    property bool pressed: overlay.recording || capture.pressed ||
        zoomSlider.pressed || modeButton.pressed

    signal previewAvailable(string uri)

    anchors.fill: parent

    VideoMode {
        id: videoMode
        camera: cam
        onPreviewAvailable: overlay.previewAvailable(preview)
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
        visible: controlsVisible && !overlay.recording
    }

    ZoomCaptureButton {
        id: zoomCapture
        onReleased: overlay.toggleRecording()
    }

    ZoomCaptureCancel {
        anchors.fill: parent
        zoomCapture: zoomCapture
    }

    CaptureButton {
        id: capture
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        iconSource: overlay.recording ? "image://theme/icon-m-camera-video-record" : "image://theme/icon-m-camera-video-record"
        width: 75
        height: 75
        opacity: 0.5

        onClicked: overlay.toggleRecording()

        visible: controlsVisible && (!settings.zoomAsShutter && keys.active)
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
// TODO: hide when recording
                onClicked: toolBar.push(items)
            },
            VideoEvCompButton {
                onClicked: toolBar.push(items)
            },
            VideoWhiteBalanceButton {
                onClicked: toolBar.push(items)
            },
            VideoColorFilterButton {
                onClicked: toolBar.push(items)
            },
            VideoMuteButton {
            }
        ]
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
                source: "image://theme/" + Data.videoIcon(settings.videoResolution)
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

    DisplayState {
        inhibitDim: overlay.recording
    }

    Connections {
        target: Qt.application
        onActiveChanged: {
            if (!Qt.application.active && overlay.recording) {
                overlay.stopRecording()
            }
        }
    }

    Timer {
        id: recordingDuration
        property int duration: 0
        running: overlay.recording
        interval: 1000
        repeat: true

        onTriggered: {
            duration = duration + 1

            if (duration == 3600) {
                overlay.stopRecording()
                showError(qsTr("Maximum recording time reached."))
            } else if (!checkDiskSpace()) {
                page.stopRecording()
                showError(qsTr("Not enough space to continue recording."))
            }

        }
    }

    RecordingDurationLabel {
        visible: overlay.recording
        duration: recordingDuration.duration
    }

    function doStartRecording() {
        if (!overlay.recording) {
            return
        }

        if (!pipelineManager.acquired || pipelineManager.hijacked) {
            showError(qsTr("Failed to acquire needed resources."))
            overlay.recording = false
            return
        }

        metaData.setMetaData()

        if (!mountProtector.lock()) {
            showError(qsTr("Failed to lock images directory."))
            overlay.recording = false
            return
        }

        var file = fileNaming.videoFileName()
        var tmpFile = fileNaming.temporaryVideoFileName()

        if (!videoMode.startRecording(file, tmpFile)) {
            showError(qsTr("Failed to record video. Please restart the camera."))
            mountProtector.unlock()
            overlay.recording = false
            return
        }

        trackerStore.storeVideo(file);
    }

    function startRecording() {
        if (!fileSystem.available) {
            showError(qsTr("Camera cannot record videos in mass storage mode."))
        } else if (!checkBattery()) {
            showError(qsTr("Not enough battery to record video."))
        } else if (!checkDiskSpace()) {
            showError(qsTr("Not enough space to record video."))
        } else {
            recordingDuration.duration = 0
            overlay.recording = true
            doStartRecording()
        }
    }

    function stopRecording() {
        videoMode.stopRecording(true)
        mountProtector.unlock()
        overlay.recording = false
    }

    function toggleRecording() {
        if (overlay.recording) {
            overlay.stopRecording()
        } else {
            overlay.startRecording()
        }
    }

    function cameraError() {
        overlay.stopRecording()
    }

    function policyLost() {
        overlay.stopRecording()
    }

    function batteryLow() {
        if (!overlay.recording) {
            return
        }

        if (!checkBattery()) {
            overlay.stopRecording()
            showError(qsTr("Not enough battery to record video."))
        }
    }

}
