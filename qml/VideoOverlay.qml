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

BaseOverlay {
    id: overlay
    property bool recording: false

    policyMode: recording == true ? CameraResources.Recording : CameraResources.Video
    pressed: overlay.recording || pageBeingManipulated
    inhibitDim: recording
    captureButtonIconSource: overlay.recording ? cameraTheme.captureButtonStopRecordingIconId : cameraTheme.captureButtonVideoIconId
    overlayCapturing: recording
    zoomSliderVisible: true
    canCapture: true

    VideoMode {
        id: videoMode
        camera: cam
        enablePreview: settings.enablePreview
        onPreviewAvailable: overlay.previewAvailable(preview)
    }

    CaptureButton {
        reversed: !settings.leftHandedMode
        iconSource: cameraTheme.videoPauseIconId
        visible: controlsVisible && overlay.recording
        onClicked: videoMode.pauseRecording(!videoMode.paused)

        PropertyAnimation on opacity  {
            easing.type: Easing.OutSine
            loops: Animation.Infinite
            from: 0.1
            to: 0.5
            duration: 1000
            running: videoMode.paused
            alwaysRunToEnd: true
        }
    }

    CameraToolBarLabel {
        id: selectedLabel
        anchors {
            bottom: toolBar.top
            bottomMargin: cameraStyle.padding
        }
    }

    CameraToolBar {
        id: toolBar
        anchors {
            bottom: parent.bottom
            bottomMargin: cameraStyle.padding
            left: parent.left
            leftMargin: cameraStyle.padding
        }
        opacity: 0.5
        targetWidth: parent.width - (anchors.leftMargin * 2)
        visible: controlsVisible
        expanded: settings.showToolBar
        onExpandedChanged: settings.showToolBar = expanded;

        tools: CameraToolBarTools {
            CameraToolIcon {
                visible: deviceFeatures().isVideoTorchSupported
                iconSource: deviceSettings().videoTorchOn ? cameraTheme.cameraTorchOnIconId : cameraTheme.cameraTorchOffIconId
                onClicked: deviceSettings().videoTorchOn = !deviceSettings().videoTorchOn
            }

            CameraToolIcon {
                visible: deviceFeatures().numberOfVideoSceneModes > 1 && !overlay.recording
                iconSource: cameraTheme.videoSceneModeIcon(deviceSettings().videoSceneMode)
                onClicked: toolBar.push(Qt.resolvedUrl("VideoSceneButton.qml"), {"selectedLabel": selectedLabel})
            }

            CameraToolIcon {
                iconSource: deviceSettings().videoEvComp == 0 ? cameraTheme.cameraManualExposureIconId : ""
                onClicked: toolBar.push(Qt.resolvedUrl("VideoEvCompButton.qml"))

                CameraLabel {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    visible: deviceSettings().videoEvComp != 0
                    text: deviceSettings().videoEvComp == 0 ? "" : deviceSettings().videoEvComp.toFixed(1)
                }
            }

            CameraToolIcon {
                iconSource: cameraTheme.whiteBalanceIcon(deviceSettings().videoWhiteBalance)
                onClicked: toolBar.push(Qt.resolvedUrl("VideoWhiteBalanceButton.qml"), {"selectedLabel": selectedLabel})
                visible: deviceFeatures().numberOfVideoWhiteBalanceModes > 1
            }

            CameraToolIcon {
                iconSource: cameraTheme.colorFilterIcon(deviceSettings().videoColorFilter)
                onClicked: toolBar.push(Qt.resolvedUrl("VideoColorFilterButton.qml"), {"selectedLabel": selectedLabel})
                visible: deviceFeatures().numberOfVideoColorTones > 1
            }

            CameraToolIcon {
                iconSource: deviceSettings().videoMuted ? cameraTheme.soundMuteOnIconId : cameraTheme.soundMuteOffIconId
                onClicked: deviceSettings().videoMuted = !deviceSettings().videoMuted
            }

            RecordingDurationLabel {
                visible: overlay.recording
                duration: recordingDuration.duration
            }

            DeviceSelector {
                visible: !overlay.recording
                    && activePlugin.primaryCameraSupported
                    && activePlugin.secondaryCameraSupported
            }
        }
    }

    Row {
        anchors {
            top: parent.top
            topMargin: cameraStyle.padding
            left: settings.leftHandedMode ? undefined: parent.left
            leftMargin: cameraStyle.padding
            right: settings.leftHandedMode ? parent.right : undefined
            rightMargin: cameraStyle.padding
        }

        height: cameraStyle.indicatorArrayHeight
        spacing: cameraStyle.spacingSmall
        visible: controlsVisible

        CameraLabel {
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: videoMode.currentResolution.commonName
            font.bold: true
            height: parent.height
        }

        Image {
            id: gpsIndicator
            height: parent.height
            width: height
            asynchronous: true
            visible: settings.useGps
            source: cameraTheme.gpsIndicatorIcon

            PropertyAnimation on opacity  {
                easing.type: Easing.OutSine
                loops: Animation.Infinite
                from: 0.2
                to: 1.0
                duration: 1000
                running: settings.useGps && !positionSource.position.horizontalAccuracyValid && viewfinder.camera.running
                alwaysRunToEnd: true
            }
        }
    }

    Connections {
        target: rootWindow
        onActiveChanged: {
            if (!rootWindow.active && overlay.recording) {
                overlay.stopRecording()
            }
        }
    }

    Connections {
        target: batteryMonitor
        onGoodChanged: {
            if (!batteryMonitor.good) {
                overlay.batteryLow()
            }
        }
    }

    Timer {
        id: recordingDuration
        property int duration: 0
        running: overlay.recording && !videoMode.paused
        interval: 1000
        repeat: true

        onTriggered: {
            duration = duration + 1

            if (platformSettings.maximumVideoDuration != -1 &&
                duration >= platformSettings.maximumVideoDuration) {
                overlay.stopRecording()
                showError(qsTr("Maximum recording time reached."))
            } else if (!fileSystem.hasFreeSpace(platformSettings.temporaryVideoPath)) {
                page.stopRecording()
                showError(qsTr("Not enough space to continue recording."))
            }
        }
    }

    function resetToolBar() {
        if (toolBar.depth() > 1) {
            toolBar.pop()
        }
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

        if (!mountProtector.lock(platformSettings.temporaryVideoPath)) {
            showError(qsTr("Failed to lock temporary videos directory."))
            overlay.recording = false
            return
        }

        if (!mountProtector.lock(platformSettings.videoPath)) {
            showError(qsTr("Failed to lock videos directory."))
            overlay.recording = false
            mountProtector.unlockAll()
            return
        }

        var file = fileNaming.videoFileName()
        var tmpFile = fileNaming.temporaryVideoFileName()

        if (!videoMode.startRecording(file, tmpFile)) {
            showError(qsTr("Failed to record video. Please restart the camera."))
            mountProtector.unlockAll()
            overlay.recording = false
            return
        }

        trackerStore.storeVideo(file);

        resetToolBar()
    }

    function startRecording() {
        if (!fileSystem.available) {
            showError(qsTr("Camera cannot record videos in mass storage mode."))
        } else if (!batteryMonitor.good) {
            showError(qsTr("Not enough battery to record video."))
        } else if (!fileSystem.hasFreeSpace(platformSettings.videoPath) || !fileSystem.hasFreeSpace(platformSettings.temporaryVideoPath)) {
            showError(qsTr("Not enough space to record video."))
        } else {
            recordingDuration.duration = 0
            overlay.recording = true
            doStartRecording()
        }
    }

    function stopRecording() {
        videoMode.stopRecording(true)
        mountProtector.unlockAll()
        overlay.recording = false
    }

    function startCapture() {
        if (overlay.recording) {
            overlay.stopRecording()
        } else {
            overlay.startRecording()
        }
    }

    function stopCapture() {
        // This is a callback needed by CaptureControln when user cancels
        // an attempt to capture. It's relevant only for images
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

        if (!batteryMonitor.good) {
            overlay.stopRecording()
            showError(qsTr("Not enough battery to record video."))
        }
    }

    function cameraDeviceChanged() {
        resetToolBar()
    }

    function applySettings() {
        var s = deviceSettings()

        camera.scene.value = s.videoSceneMode
        camera.evComp.value = s.videoEvComp
        camera.whiteBalance.value = s.videoWhiteBalance
        camera.colorTone.value = s.videoColorFilter
        camera.videoMute.enabled = s.videoMuted
        camera.videoTorch.on = s.videoTorchOn
        camera.focus.value = Focus.ContinuousNormal

        videoSettings.setVideoResolution()
    }
}
