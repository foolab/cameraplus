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
import com.nokia.extras 1.1
import QtCamera 1.0
import CameraPlus 1.0
import QtMobility.location 1.2
//import QtCamera 1.0
// TODO: flash not ready (battery low or flash not ready message)

Window {
        id: root
        property alias camera: cam
//        property bool cameraVisible: mainView.currentIndex == 1

        VisualItemModel {
            id: mainModel

            SettingsView {
                width: mainView.width
                height: mainView.height
            }

            CameraView {
                id: cam
                width: mainView.width
                height: mainView.height
            }

            PostCaptureView {
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
            screen.setAllowedOrientations(Screen.Landscape)
            theme.inverted = true
            platformSettings.init()        
            // TODO: hardcoding device id
            root.resetCamera(0, settings.mode)
        }

        PlatformSettings {
            id: platformSettings
        }

        Settings {
            id: settings
        }

        PipelineManager {
            id: pipelineManager
            camera: cam
            currentItem: mainView.currentItem
        }

        function resetCamera(deviceId, mode) {
            if (!cam.reset(deviceId, mode)) {
                showError(qsTr("Failed to set camera device and mode. Please restart the application."))
            }
        }

        function showError(msg) {
            error.text = msg
            error.show()
        }

        property alias dimmer: camDimmer

        PageStack {
            id: pageStack
            anchors.fill: parent
        }

        MouseArea {
            anchors.fill: parent
            enabled: pageStack.busy
        }

        PositionSource {
            // NOTE: The source will not reset the position when we lose the signal.
            // This shouldn't be a big problem as we are course enough.
            // If we ever need street level updates then this will be an issue.
            id: positionSource
            active: settings.useGps
            // TODO: we cannot bind to cam.running because camera will stop
            // when the connection dialog pops up and we end up with an infinite loop
            // active: cam.running && settings.useGps
            onPositionChanged: geocode.search(position.coordinate.longitude, position.coordinate.latitude)
        }

        MetaData {
            id: metaData
            camera: cam
            manufacturer: deviceInfo.manufacturer
            model: deviceInfo.model
            country: geocode.country
            city: geocode.city
            suburb: geocode.suburb
            longitude: positionSource.position.coordinate.longitude
            longitudeValid: positionSource.position.longitudeValid && settings.useGps
            latitude: positionSource.position.coordinate.latitude
            latitudeValid: positionSource.position.latitudeValid && settings.useGps
            elevation: positionSource.position.coordinate.altitude
            elevationValid: positionSource.position.altitudeValid && settings.useGps
            orientation: orientation.orientation
            artist: settings.creatorName
            captureDirection: compass.direction
            captureDirectionValid: compass.directionValid
            horizontalError: positionSource.position.horizontalAccuracy
            horizontalErrorValid: positionSource.position.horizontalAccuracyValid && settings.useGps
            dateTimeEnabled: true
        }

        Orientation {
            id: orientation
            active: cam.running
        }

        Compass {
            id: compass
            active: cam.running
        }

        ReverseGeocode {
            id: geocode
            active: cam.running && settings.useGps && settings.useGeotags
        }

        DeviceInfo {
            id: deviceInfo
        }

        FSMonitor {
            id: fileSystem
        }

        InfoBanner {
            id: error
        }

        FileNaming {
            id: fileNaming
            imageSuffix: cam.imageSuffix
            videoSuffix: cam.videoSuffix
        }

        MountProtector {
            id: mountProtector
            path: fileNaming.path
        }

        TrackerStore {
            id: trackerStore
            active: cam.running
            manufacturer: deviceInfo.manufacturer
            model: deviceInfo.model
        }

        function checkDiskSpace() {
            return fileSystem.hasFreeSpace(fileNaming.path)
        }

        ImageSettings {
            id: imageSettings
            camera: cam
            function setImageResolution() {
                if (!imageSettings.setResolution(settings.imageAspectRatio, settings.imageResolution)) {
                    showError(qsTr("Failed to set required resolution"))
                }
            }

            onReadyChanged: {
                if (ready) {
                    imageSettings.setImageResolution()
                }
            }
        }

        VideoSettings {
            id: videoSettings
            camera: cam

            function setVideoResolution() {
                if (!videoSettings.setResolution(settings.videoAspectRatio, settings.videoResolution)) {
                    showError(qsTr("Failed to set required resolution"))
                }
            }

            onReadyChanged: {
                if (ready) {
                    videoSettings.setVideoResolution()
                }
            }
        }

        Connections {
            target: settings

            onImageAspectRatioChanged: {
                imageSettings.setImageResolution()
            }

            onImageResolutionChanged: {
                imageSettings.setImageResolution()
            }

            onVideoResolutionChanged: {
                videoSettings.setVideoResolution()
            }
        }

        ModeController {
            id: cameraMode
            cam: cam
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

//        Standby {
//            policyLost: pipelineManager.state == "policyLost"
// TODO:
//            show: !pageStack.currentPage || (pageStack.currentPage.activationData.standbyVisible && pageStack.currentPage.status == PageStatus.Active && pipelineManager.showStandBy)
//        }
}
