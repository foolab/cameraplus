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

CameraPage {
    id: root

    property bool deviceChangeInProgress: false

    CameraTheme {
        id: cameraTheme
    }

    VisualItemModel {
        id: mainModel

        SettingsView {
            camera: viewfinder.camera
            width: mainView.width
            height: mainView.height
        }

        CameraView {
            id: viewfinder
            width: mainView.width
            height: mainView.height
        }

        PostCaptureView {
            camera: viewfinder.camera
            width: mainView.width
            height: mainView.height
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

        onVideoAspectRatioChanged: {
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

    property alias dimmer: camDimmer
    CameraPositionSource {
        id: positionSource
        active: settings.useGps
        // TODO: we cannot bind to cam.running because camera will stop
        // when the connection dialog pops up and we end up with an infinite loop
        // active: cam.running && settings.useGps
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
        active: viewfinder.camera.running || (mainView.currentIndex == 2 && Qt.application.active)
    }

    CameraCompass {
        id: compass
        active: viewfinder.camera.running
    }

    ReverseGeocode {
        id: geocode
        active: viewfinder.camera.running && settings.useGps && settings.useGeotags
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
        imageSuffix: viewfinder.camera.imageSuffix
        videoSuffix: viewfinder.camera.videoSuffix
        imagePath: platformSettings.imagePath
        videoPath: platformSettings.videoPath
        temporaryVideoPath: platformSettings.temporaryVideoPath
        settings: settings
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
        id: cameraMode
        cam: viewfinder.camera
        dimmer: root.dimmer
    }

    Rectangle {
        property bool dimmed: false
        id: camDimmer
        z: 1
        anchors.fill: parent
        opacity: dimmed ? 1.0 : 0.0
        color: "black"
        Behavior on opacity {
            PropertyAnimation { duration: 150 }
        }
    }

    DeviceKeys {
        id: keys
        active: Qt.application.active && pipelineManager.scaleAcquired
        repeat: !settings.zoomAsShutter
    }

    Standby {
        policyLost: pipelineManager.state == "policyLost"
        show: !Qt.application.active || pipelineManager.showStandBy ||
            (mainView.currentIndex == 1 && !viewfinder.camera.running)
    }
}
