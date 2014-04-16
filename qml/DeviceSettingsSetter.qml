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

Item {
    id: setter
    // I don't know why but seems having Connections as the root element breaks something
    // And I get an error qrc:/qml/MainPage.qml:123: ReferenceError: Can't find variable: camera
    // whenever a settings property changes
    property QtObject settings
    property Camera camera
    property ImageSettings imageSettings
    property VideoSettings videoSettings

    // on scene mode changes -> commit to device and update all props from device
    function resetVideoSceneMode() {
        camera.scene.value = settings.videoSceneMode

        settings.videoEvComp = camera.evComp.value
        settings.videoWhiteBalance = camera.whiteBalance.value
        settings.videoColorFilter = camera.colorTone.value
    }

    // on scene mode changes -> commit to device and update all props from device
    function resetImageSceneMode() {
        camera.scene.value = settings.imageSceneMode

        settings.imageFlashMode = camera.flash.value
        settings.imageEvComp = camera.evComp.value
        settings.imageWhiteBalance = camera.whiteBalance.value
        settings.imageColorFilter = camera.colorTone.value
        settings.imageIso = camera.iso.value
    }

    function resetSceneMode() {
        if (settings.mode == Camera.VideoMode) {
            resetVideoSceneMode()
        } else {
            resetImageSceneMode()
        }
    }

    Connections {
        target: settings

        onImageSceneModeChanged: setter.resetImageSceneMode()
        onVideoSceneModeChanged: setter.resetVideoSceneMode()

        // on property changes -> commit to device
        onImageColorFilterChanged: camera.colorTone.value = settings.imageColorFilter
        onVideoColorFilterChanged: camera.colorTone.value = settings.videoColorFilter

        onImageWhiteBalanceChanged: camera.whiteBalance.value = settings.imageWhiteBalance
        onVideoWhiteBalanceChanged: camera.whiteBalance.value = settings.videoWhiteBalance

        onImageEvCompChanged: camera.evComp.value = settings.imageEvComp
        onVideoEvCompChanged: camera.evComp.value = settings.videoEvComp

        onImageFlashModeChanged: camera.flash.value = settings.imageFlashMode
        onImageIsoChanged: camera.iso.value = settings.imageIso

        onVideoTorchOnChanged: camera.videoTorch.on = settings.videoTorchOn
        onVideoMutedChanged: camera.videoMute.enabled = settings.videoMuted

        onImageAspectRatioChanged: imageSettings.setImageResolution()
        onImageResolutionChanged: imageSettings.setImageResolution()
        onVideoResolutionChanged: videoSettings.setVideoResolution()
    }
}
