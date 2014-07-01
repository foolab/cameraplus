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

CameraPage {
    id: root

    property bool inCaptureMode: mainView.currentIndex == 1
    property Plugin activePlugin

    Connections {
        target: rootWindow
        onActiveChanged: {
            if (!rootWindow.active) {
                mainView.currentIndex = 1
            }

            rootWindow.popAll()
        }
    }

    CameraStyle {
        id: cameraStyle
    }

    PluginLoader {
        id: plugins
    }

    CameraTheme {
        id: cameraTheme
    }

    VisualItemModel {
        id: mainModel

        Loader {
            id: settingsLoader
            width: mainView.width
            height: mainView.height

            property bool pressed
            property bool inhibitDim
            property int policyMode: activePlugin.mode == Camera.VideoMode ? CameraResources.Video : CameraResources.Image
            opacity: item ? 1.0 : 0

            Behavior on opacity {
                NumberAnimation {duration: 200}
            }
        }

        CameraView {
            id: viewfinder
            width: mainView.width
            height: mainView.height
        }

        Loader {
            id: postCaptureLoader
            property bool pressed: item ? item.pressed : false
            property bool inhibitDim: item ? item.inhibitDim : false
            property int policyMode: item ? item.policyMode : activePlugin.mode == Camera.VideoMode ? CameraResources.Video : CameraResources.Image

            width: mainView.width
            height: mainView.height
            opacity: item ? 1.0 : 0

            Behavior on opacity {
                NumberAnimation {duration: 200}
            }
        }
    }

    ListView {
        id: mainView
        LayoutMirroring.enabled: false
        anchors.fill: parent
        orientation: ListView.Horizontal
        model: mainModel
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        highlightMoveDuration: 0
        boundsBehavior: Flickable.StopAtBounds
        currentIndex: 1
        interactive: !currentItem.pressed
        onContentXChanged: {
            if (contentX == 0) {
                settingsLoader.source = Qt.resolvedUrl("SettingsView.qml")
            } else if (contentX == width) {
                settingsLoader.source = ""
                postCaptureLoader.source = ""
            } else if (contentX == width * 2) {
                postCaptureLoader.source = Qt.resolvedUrl("PostCaptureView.qml")
            }
        }
    }

    Component.onCompleted: {
        platformSettings.init()
        root.resetCamera(settings.device)
    }

    PlatformSettings {
        id: platformSettings
    }

    PrimaryDeviceSettings {
        id: primarySettings
        settings: settings
    }

    DeviceSettingsSetter {
        id: primarySetter
        settings: primarySettings
        camera: viewfinder.camera
        imageSettings: imageSettings
        videoSettings: videoSettings
    }

    SecondaryDeviceSettings {
        id: secondarySettings
        settings: settings
    }

    DeviceSettingsSetter {
        id: secondarySetter
        settings: secondarySettings
        camera: viewfinder.camera
        imageSettings: imageSettings
        videoSettings: videoSettings
    }

    PrimaryDeviceFeatures {
        id: primaryDeviceFeatures
    }

    SecondaryDeviceFeatures {
        id: secondaryDeviceFeatures
    }

    Settings {
        id: settings
    }

    PipelineManager {
        id: pipelineManager
        camera: viewfinder.camera
        currentItem: mainView.currentItem
        displayOn: displayState.isOn
    }

    function deviceSettings() {
        return viewfinder.camera.deviceId == 0 ? primarySettings : secondarySettings
    }

    function deviceSettingsSetter() {
        return viewfinder.camera.deviceId == 0 ? primarySetter : secondarySetter
    }

    function deviceFeatures() {
        return viewfinder.camera.deviceId == 0 ? primaryDeviceFeatures : secondaryDeviceFeatures
    }

    function resetCamera(deviceId) {
        // load the plugin:
        if (!activePlugin || !activePlugin.valid || activePlugin.uuid != settings.plugin) {
            var newPlugin = plugins.get(settings.plugin)
            if (!newPlugin || !newPlugin.valid) {
                // We failed. Try to load the image plugin.
                settings.plugin = "org.foolab.cameraplus.image"
                newPlugin = plugins.get("org.foolab.cameraplus.image")
                if (!newPlugin || !newPlugin.valid) {
                    showError(qsTr("Failed to load required camera plugin. Please reinstall!"))
                    return false
                }
            }

            activePlugin = newPlugin
        }

        if (!viewfinder.camera.reset(deviceId, activePlugin.mode)) {
            showError(qsTr("Failed to initialize camera. Please restart."))
            return false
        }

        var s = deviceSettings()
        if (activePlugin.mode == Camera.ImageMode) {
            viewfinder.camera.scene.value = s.imageSceneMode
            viewfinder.camera.flash.value = s.imageFlashMode
            viewfinder.camera.evComp.value = s.imageEvComp
            viewfinder.camera.whiteBalance.value = s.imageWhiteBalance
            viewfinder.camera.colorTone.value = s.imageColorFilter
            viewfinder.camera.iso.value = s.imageIso
            viewfinder.camera.focus.value = Focus.ContinuousNormal

            imageSettings.setImageResolution()
        } else {
            viewfinder.camera.scene.value = s.videoSceneMode
            viewfinder.camera.evComp.value = s.videoEvComp
            viewfinder.camera.whiteBalance.value = s.videoWhiteBalance
            viewfinder.camera.colorTone.value = s.videoColorFilter
            viewfinder.camera.videoMute.enabled = s.videoMuted
            viewfinder.camera.videoTorch.on = s.videoTorchOn
            viewfinder.camera.focus.value = Focus.ContinuousNormal

            videoSettings.setVideoResolution()
        }

        return true
    }

    function showError(msg) {
        error.text = msg
        console.log("Error: " + msg);
        error.show()
    }

    DisplayState {
        id: displayState
        inhibitDim: mainView.currentItem != null ? mainView.currentItem.inhibitDim : false
    }

    CameraPositionSource {
        id: positionSource
        active: viewfinder.camera.running && settings.useGps
        onPositionChanged: {
            if (positionSource.horizontalAccuracyValid) {
                geocode.search(position.coordinate.longitude, position.coordinate.latitude)
            } else {
                geocode.clear()
            }
        }
    }

    Position {
        id: cameraPosition
        property int primarySensorOrientationAngle: platformSettings.primarySensorOrientationAngle == -1 ? viewfinder.camera.sensorOrientationAngle : platformSettings.primarySensorOrientationAngle
        property int secondarySensorOrientationAngle: platformSettings.secondarySensorOrientationAngle == -1 ? viewfinder.camera.sensorOrientationAngle : platformSettings.secondarySensorOrientationAngle
        deviceOrientationAngle: orientation.orientationAngle
        sensorOrientationAngle: frontCamera ? secondarySensorOrientationAngle : primarySensorOrientationAngle
        frontCamera: viewfinder.camera.deviceId == 1
        applicationOrientationAngle: orientationAngle
        naturalOrientation: platformSettings.naturalOrientationIsLandscape ? Position.Landscape : Position.Portrait
    }

    MetaData {
        id: metaData
        camera: viewfinder.camera
        manufacturer: deviceInfo.manufacturer
        model: deviceInfo.model
        country: geocode.country
        city: geocode.city
        suburb: geocode.suburb
        longitude: positionSource.longitude
        longitudeValid: positionSource.longitudeValid && settings.useGps
        latitude: positionSource.latitude
        latitudeValid: positionSource.latitudeValid && settings.useGps
        elevation: positionSource.altitude
        elevationValid: positionSource.altitudeValid && settings.useGps
        orientationAngle: cameraPosition.exifRotationAngle
        artist: settings.creatorName
        captureDirection: compass.direction
        captureDirectionValid: compass.directionValid
        horizontalError: positionSource.horizontalAccuracy
        horizontalErrorValid: positionSource.horizontalAccuracyValid && settings.useGps
        dateTimeEnabled: true
    }

    CameraOrientation {
        id: orientation
        active: viewfinder.camera.running || (mainView.currentIndex == 2 && rootWindow.active)
    }

    CameraCompass {
        id: compass
        active: viewfinder.camera.running
    }

    ReverseGeocode {
        id: geocode
        active: settings.useGeotags
    }

    DeviceInfo {
        id: deviceInfo
    }

    FSMonitor {
        id: fileSystem
    }

    CameraInfoBanner {
        id: error
    }

    FileNaming {
        id: fileNaming
        imageSuffix: viewfinder.camera.cameraConfig.imageSuffix
        videoSuffix: viewfinder.camera.cameraConfig.videoSuffix
        settings: settings
        platformSettings: platformSettings
    }

    MountProtector {
        id: mountProtector
    }

    TrackerStore {
        id: trackerStore
        active: viewfinder.camera.running
        manufacturer: deviceInfo.manufacturer
        model: deviceInfo.model
    }

    ImageSettings {
        id: imageSettings
        camera: viewfinder.camera

        function setImageResolution() {
            if (!imageSettings.setResolution(deviceSettings().imageResolution)) {
                showError(qsTr("Failed to set required resolution"))
            }
        }
    }

    VideoSettings {
        id: videoSettings
        camera: viewfinder.camera

        function setVideoResolution() {
            if (!videoSettings.setResolution(deviceSettings().videoResolution)) {
                showError(qsTr("Failed to set required resolution"))
            }
        }
    }

    ModeController {
        id: modeController
        cam: viewfinder.camera
    }

    DeviceKeys {
        id: keys
        active: rootWindow.active && pipelineManager.scaleAcquired && root.inCaptureMode && !mainView.moving
        repeat: !settings.zoomAsShutter
    }

    Timer {
        id: proximityTimer
        running: proximitySensor.close
        repeat: false
        interval: 500
        onTriggered: {
            if (proximitySensor.close) {
                proximitySensor.sensorClosed = true
            }
        }
    }

    CameraProximitySensor {
        id: proximitySensor
        property bool sensorClosed: false

        active: rootWindow.active && viewfinder.camera.running && settings.proximityAsShutter && root.inCaptureMode && !mainView.moving
        onCloseChanged: {
            if (!close) {
                sensorClosed = false
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        enabled: pluginSelector.source != ""
        onClicked: pluginSelector.hide()
    }

    Loader {
        id: pluginSelector
        property bool _show
        anchors.centerIn: parent
        width: parent.width - 40
        height: 150
        opacity: _show ? 0.8 : 0

        Behavior on opacity {
            SequentialAnimation {
                NumberAnimation { duration: 200; }
                ScriptAction { script: if (!pluginSelector._show) { pluginSelector.source = "" } }
            }
        }

        function show() {
            _show = true
            pluginSelector.source = Qt.resolvedUrl("PluginSelector.qml")
        }

        function hide() {
            _show = false
        }

    }

    Standby {
        policyLost: pipelineManager.state == "policyLost"
        visible: !rootWindow.active || pipelineManager.showStandBy ||
            (inCaptureMode && !viewfinder.camera.running)
    }
}
