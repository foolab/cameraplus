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

// TODO: flash not ready (battery low or flash not ready message)
// TODO: Sounds
// TODO: N950 conf
// TODO: show busy indicators while loaders are loading
// TODO: slider is loading crap too
// TODO: split bindings per mode (image & video)
// TODO: flash setting does not survive restarts
// TODO: async policy
// TODO: portrait layout

CameraPage {
    id: root

    property bool deviceChangeInProgress
    property bool inCaptureMode: mainView.currentIndex == 1

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
            property int policyMode: settings.mode == Camera.VideoMode ? CameraResources.Video : CameraResources.Image
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
            property int policyMode: item ? item.policyMode : settings.mode == Camera.VideoMode ? CameraResources.Video : CameraResources.Image

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
        root.resetCamera(settings.device, settings.mode)
    }

    PlatformSettings {
        id: platformSettings
    }

    Settings {
        id: settings
        onDeviceAboutToChange: {
            root.deviceChangeInProgress = true
        }

        onDeviceChanged: {
            viewfinder.cameraDeviceChanged()

            // Reset pipeline manager error
            pipelineManager.error = false

            if (root.resetCamera(settings.device, settings.mode)) {
                root.deviceChangeInProgress = false
                pipelineManager.startCamera()
            }
        }

        onImageAspectRatioChanged: {
            if (!root.deviceChangeInProgress) {
                imageSettings.setImageResolution()
            }
        }

        onImageResolutionChanged: {
            if (!root.deviceChangeInProgress) {
                imageSettings.setImageResolution()
            }
        }

        onVideoResolutionChanged: {
            if (!root.deviceChangeInProgress) {
                videoSettings.setVideoResolution()
            }
        }
    }

    PipelineManager {
        id: pipelineManager
        camera: viewfinder.camera
        currentItem: mainView.currentItem
    }

    function resetCamera(deviceId, mode) {
        if (!viewfinder.camera.reset(deviceId, mode)) {
            showError(qsTr("Failed to set camera device and mode. Please restart the application."))
            return false
        }
// TODO: set resolution before the mode?
        if (mode == Camera.ImageMode) {
            imageSettings.setImageResolution()
        }
        else if (mode == Camera.VideoMode) {
            videoSettings.setVideoResolution()
        }

        return true
    }

    function showError(msg) {
        error.text = msg
        error.show()
    }

    DisplayState {
        id: displayState
        inhibitDim: mainView.currentItem != null ? mainView.currentItem.inhibitDim : false
    }

    CameraPositionSource {
        id: positionSource
// TODO: isOn needed?
        active: viewfinder.camera.running && settings.useGps && displayState.isOn
        onPositionChanged: geocode.search(position.coordinate.longitude, position.coordinate.latitude)
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
        orientation: orientation.orientation
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
// TODO: isOn needed?
        active: viewfinder.camera.running && settings.useGps && settings.useGeotags && displayState.isOn
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
            if (!imageSettings.setResolution(settings.imageAspectRatio, settings.imageResolution)) {
                showError(qsTr("Failed to set required resolution"))
            }
        }
    }

    VideoSettings {
        id: videoSettings
        camera: viewfinder.camera

        function setVideoResolution() {
            if (!videoSettings.setResolution(settings.videoAspectRatio, settings.videoResolution)) {
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

    Standby {
        policyLost: pipelineManager.state == "policyLost"
        visible: !rootWindow.active || pipelineManager.showStandBy ||
            (inCaptureMode && !viewfinder.camera.running)
    }
}
