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
import com.nokia.extras 1.1
import QtCamera 1.0
import CameraPlus 1.0
import QtMobility.systeminfo 1.2
import QtMobility.location 1.2

// TODO: postcapture
// TODO: flash not ready (battery low or flash not ready message)
// TODO: portrait/landscape
// TODO: grid lines, face tracking
// TODO: select primary/secondary camera.
// TODO: disable debug builds.
// TODO: a way to get buffers to the application
// TODO: fcam like functionality (precise control over capture parameters).
// TODO: mute video sound

PageStackWindow {
        id: root

        property alias dimmer: camDimmer

        showStatusBar: false

        Component.onCompleted: {
                theme.inverted = true;
                // TODO: hardcoding device id
                root.resetCamera(0, settings.mode);
        }

        function showError(msg) {
                error.text = msg;
                error.show();
        }

        function resetCamera(deviceId, mode) {
                if (!cam.reset(deviceId, mode)) {
                        showError(qsTr("Failed to set camera device and mode. Please restart the application."));
                }
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
                onPositionChanged: geocode.search(position.coordinate.longitude, position.coordinate.latitude);
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

        PipelineManager {
                id: pipelineManager
                camera: cam
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

        Settings {
                id: settings
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

        function replacePage(file) {
                pageStack.replace(Qt.resolvedUrl(file), {cam: cam}, true);
        }

        function openFile(file) {
                pageStack.push(Qt.resolvedUrl(file), {cam: cam});
        }

        function openFileNow(file) {
                pageStack.push(Qt.resolvedUrl(file), {cam: cam}, true);
        }

        platformStyle: PageStackWindowStyle {
                cornersVisible: false
                background: ""
                backgroundColor: "transparent"
        }

        ImageSettings {
                id: imageSettings
                camera: cam
                function setImageResolution() {
                        if (!imageSettings.setResolution(settings.imageAspectRatio, settings.imageResolution)) {
                                showError(qsTr("Failed to set required resolution"));
                        }
                }

                onReadyChanged: {
                        if (ready) {
                                imageSettings.setImageResolution();
                        }
                }
        }

        VideoSettings {
                id: videoSettings
                camera: cam

                function setVideoResolution() {
                        if (!videoSettings.setResolution(settings.videoAspectRatio, settings.videoResolution)) {
                                showError(qsTr("Failed to set required resolution"));
                        }
                }

                onReadyChanged: {
                        if (ready) {
                                videoSettings.setVideoResolution();
                        }
                }
        }

        Connections {
                target: settings

                onImageAspectRatioChanged: {
                        imageSettings.setImageResolution();
                }

                onImageResolutionChanged: {
                        imageSettings.setImageResolution();
                }

                onVideoResolutionChanged: {
                        videoSettings.setVideoResolution();
                }
        }

        Camera {
                id: cam
                anchors.fill: parent

                FocusReticle {
                        id: focusReticle
                        cam: cam
                        visible: pageStack.currentPage && pageStack.currentPage.controlsVisible && pageStack.currentPage.focusReticleVisible && cam && cam.autoFocus.canFocus(cam.scene.value);
                        cafStatus: cam ? cam.autoFocus.cafStatus : -1
                        status: cam ? cam.autoFocus.status : -1
        }

/*
                onDeviceIdChanged: {
                        // TODO: is this needed ?
                        if (platformWindow.active) {
                                cam.start();
                        }
                }
*/
                onError: {
                        if (pipelineManager.error) {
                                // Ignore any subsequent errors.
                                // Killing pulseaudio while recording will lead to an
                                // infinite supply of errors which will break the UI
                                // if we show a banner for each.
                                return;
                        }

                        pipelineManager.error = true;
                        pageStack.currentPage.cameraError();
                        console.log("Camera error (" + code + "): " + message + " " + debug);
                        showError(qsTr("Camera error. Please restart the application."));

                        // We cannot stop camera here. Seems there is a race condition somewhere
                        // which leads to a freeze if we do so.
                }

                onRunningChanged: {
                        if (!cam.running) {
                                mountProtector.unlock();
                        }
                }

                Component.onDestruction: cam.stop();

                // We need to show viewfinder below pages.
                z: -1

                Rectangle {
                        id: camDimmer
                        z: 1
                        anchors.fill: parent
                        opacity: 0
                        color: "black"
                }

                notifications: Sounds {
                        id: sounds
                        mute: !settings.soundEnabled
                }

        }

        Binding {
                target: cam.flash
                property: "value"
                when: cam.mode == Camera.ImageMode
                value: settings.imageFlashMode
        }

        Binding {
                target: settings
                property: "imageFlashMode"
                when: cam.mode == Camera.ImageMode
                value: cam.flash.value
        }

        Binding {
                target: cam.scene
                property: "value"
                when: cam.mode == Camera.VideoMode
                value: settings.videoSceneMode
        }

        Binding {
                target: cam.scene
                property: "value"
                when: cam.mode == Camera.ImageMode
                value: settings.imageSceneMode
        }

        Binding {
                target: cam.evComp
                property: "value"
                when: cam.mode == Camera.ImageMode
                value: settings.imageEvComp
        }

        Binding {
                target: cam.evComp
                property: "value"
                when: cam.mode == Camera.VideoMode
                value: settings.videoEvComp
        }

        Binding {
                target: settings
                property: "imageEvComp"
                when: cam.mode == Camera.ImageMode
                value: cam.evComp.value
        }

        Binding {
                target: settings
                property: "videoEvComp"
                when: cam.mode == Camera.VideoMode
                value: cam.evComp.value
        }

        Binding {
                target: cam.whiteBalance
                property: "value"
                when: cam.mode == Camera.ImageMode
                value: settings.imageWhiteBalance
        }

        Binding {
                target: cam.whiteBalance
                property: "value"
                when: cam.mode == Camera.VideoMode
                value: settings.videoWhiteBalance
        }

        Binding {
                target: cam.colorTone
                property: "value"
                when: cam.mode == Camera.ImageMode
                value: settings.imageColorFilter
        }

        Binding {
                target: cam.colorTone
                property: "value"
                when: cam.mode == Camera.VideoMode
                value: settings.videoColorFilter
        }

        Binding {
                target: cam.iso
                property: "value"
                when: cam.mode == Camera.ImageMode
                value: settings.imageIso
        }

        Binding {
                target: settings
                property: "imageIso"
                when: cam.mode == Camera.ImageMode
                value: cam.iso.value
        }

        TrackerStore {
                id: trackerStore
                active: cam.running
                manufacturer: deviceInfo.manufacturer
                model: deviceInfo.model
        }

        ModeController {
                id: cameraMode
                cam: cam
                dimmer: root.dimmer
        }

        Connections {
                target: cam
                onModeChanged: {
                        if (cam.mode == Camera.VideoMode) {
                                replacePage("VideoPage.qml");
                        }
                        else {
                                replacePage("ImagePage.qml");
                        }
                }
        }

        Standby {
                policyLost: pipelineManager.state == "policyLost"
                show: pageStack.currentPage.standbyVisible && pageStack.currentPage.status == PageStatus.Active && pipelineManager.showStandBy
        }
}
