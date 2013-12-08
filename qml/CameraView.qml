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

Viewfinder {
    id: viewfinder
    property bool pressed: focusReticle.locked || preview.animationRunning
        || (loader.item ? loader.item.pressed : false)
    property int policyMode: loader.item ? loader.item.policyMode : CameraResources.None
    property bool inhibitDim: loader.item ? loader.item.inhibitDim : false
    property bool inCaptureView: mainView.currentIndex == 1 && !mainView.moving

    camera: cam
    cameraConfig: cam.cameraConfig
    renderingEnabled: mainView.currentItem == viewfinder

    Component.onDestruction: cam.stop()

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: settings.nightMode ? 0.7 : 0
    }

    GridLines {
        x: viewfinder.renderArea.x
        y: viewfinder.renderArea.y
        width: viewfinder.renderArea.width
        height: viewfinder.renderArea.height
        visible: settings.gridEnabled
    }

    PhoneProfile {
        id: phoneProfile
    }

    Camera {
        id: cam
        sounds: Sounds {
            id: sounds
            mute: !settings.soundEnabled || phoneProfile.isSilent
            volume: volumeControl.fullVolume ? Sounds.VolumeHigh : Sounds.VolumeLow
            imageCaptureStart: platformSettings.imageCaptureStartedSound
            imageCaptureEnd: platformSettings.imageCaptureEndedSound
            videoRecordingStart: platformSettings.videoRecordingStartedSound
            videoRecordingEnd: platformSettings.videoRecordingEndedSound
            autoFocusAcquired: platformSettings.autoFocusAcquiredSound
        }

        onFocusChanged: focus.value = Focus.ContinuousNormal
        onRoiChanged: roi.normalize = false

        onRunningChanged: {
            if (!cam.running) {
                mountProtector.unlockAll()
            }
        }

        onError: {
            if (pipelineManager.error) {
                // Ignore any subsequent errors.
                // Killing pulseaudio while recording will lead to an
                // infinite supply of errors which will break the UI
                // if we show a banner for each.
                return
            }

            pipelineManager.error = true
            if (loader.item) {
                loader.item.cameraError()
            }

            console.log("Camera error (" + code + "): " + message + " " + debug)
            showError(qsTr("Camera error. Please restart the application."))
            // We cannot stop camera here. Seems there is a race condition somewhere
            // which leads to a freeze if we do so.
        }

    }

    SoundVolumeControl {
        id: volumeControl
    }

    BatteryInfo {
        id: batteryMonitor
        active: cam.running

        function check() {
            if (!checkBattery()) {
                loader.item.batteryLow()
            }
        }

        onChargingChanged: {
            batteryMonitor.check()
        }

        onCriticalChanged: {
            batteryMonitor.check()
        }
    }

    PreviewImage {
        id: preview
    }

    FocusReticle {
        id: focusReticle
        cam: cam
        videoResolution: viewfinder.videoResolution
        renderArea: viewfinder.renderArea

        visible: loader.item != null && loader.item.controlsVisible &&
            cam.autoFocus.canFocus(cam.scene.value)
        cafStatus: cam ? cam.autoFocus.cafStatus : -1
        status: cam ? cam.autoFocus.status : -1
    }

    Loader {
        id: loader
        property string src: cam.mode == Camera.VideoMode ? "VideoOverlay.qml" : "ImageOverlay.qml"
        anchors.fill: parent
        source: Qt.resolvedUrl(src)
    }

    Connections {
        target: loader.item
        onPreviewAvailable: {
            if (settings.enablePreview) {
                preview.setPreview(uri)
            }
        }
    }

    Binding {
        target: loader.item
        property: "cam"
        value: cam
        when: loader.item != null
    }

    Binding {
        target: loader.item
        property: "animationRunning"
        value: preview.animationRunning
        when: loader.item != null
    }

    /* Camera bindings */
    Binding {
        target: cam.roi
        property: "enabled"
        value: settings.faceDetectionEnabled && !focusReticle.pressed && !focusReticle.touchMode && cam.mode == Camera.ImageMode
    }

    function policyLost() {
        if (loader.item) {
            loader.item.policyLost()
        }
    }

    function checkBattery() {
        // We are fine if we are connected to the charger:
        if (batteryMonitor.charging) {
            return true
        }

        // If we have enough battery then we are fine:
        if (!batteryMonitor.critical) {
            return true
        }

        return false
    }

    function cameraDeviceChanged() {
        if (loader.item) {
            loader.item.cameraDeviceChanged()
        }
    }
}
