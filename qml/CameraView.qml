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

Viewfinder {
    id: viewfinder
    property bool pressed: preview.animationRunning || (loader.item ? loader.item.pressed : false)
    property int policyMode: loader.item ? loader.item.policyMode : CameraResources.None
    property bool inhibitDim: loader.item ? loader.item.inhibitDim : false
    property bool inCaptureView: mainView.currentIndex == 1 && !mainView.moving

    camera: cam
    cameraConfig: cam.cameraConfig
    renderingEnabled: mainView.currentItem == viewfinder
    viewfinderRotationAngle: cameraPosition.viewfinderRotationAngle
    viewfinderFlipped: cameraPosition.isViewfinderFlipped

    Component.onDestruction: cam.stop()

    Rectangle {
        anchors.fill: parent
        color: cameraStyle.backgroundColor
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
            autoFocusFailed: platformSettings.autoFocusFailedSound
        }

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
    }

    PreviewImage {
        id: preview
    }

    FocusReticle {
        id: focusReticle
        x: viewfinder.renderArea.x
        y: viewfinder.renderArea.y
        width: viewfinder.renderArea.width
        height: viewfinder.renderArea.height
        cam: cam
        videoResolution: viewfinder.videoResolution
        reticlePressed: mouse.pressed
        visible: loader.item != null && loader.item.controlsVisible &&
            cam.autoFocus.canFocus(cam.scene.value)
        cafStatus: cam ? cam.autoFocus.cafStatus : AutoFocus.None
        autoFocusStatus: cam ? cam.autoFocus.status : AutoFocus.None
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onDoubleClicked: focusReticle.doubleClicked()

        onPressed: {
            if (mouseX >= viewfinder.renderArea.x && mouseY >= viewfinder.renderArea.y && mouseX <= viewfinder.renderArea.x + viewfinder.renderArea.width && mouseY <= viewfinder.renderArea.y + viewfinder.renderArea.height) {
                focusReticle.pressed(mouse)
            }
        }

        onCanceled: focusReticle.canceled()
    }

    function loadPlugin(plugin) {
        var oldSource = loader.source
        loader.source = Qt.resolvedUrl(plugin.overlay)
        if (loader.status == Loader.Error) {
            console.log("Error loading " + plugin.overlay)
            loader.source = oldSource
            return false
        }

        return true
    }

    function applySettings() {
        loader.item.applySettings()
    }

    Loader {
        id: loader
        anchors.fill: parent
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
        value: deviceFeatures().isFaceDetectionSupported ? settings.faceDetectionEnabled && !focusReticle.reticlePressed && !focusReticle.touchMode && cam.mode == Camera.ImageMode : false
    }

    function policyLost() {
        if (loader.item) {
            loader.item.policyLost()
        }
    }

    function cameraDeviceChanged() {
        if (loader.item) {
            loader.item.cameraDeviceChanged()
        }
    }
}
