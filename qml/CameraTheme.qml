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

QtObject {
    property string standbyIcon: "qrc:/images/cameraplus-icon-l-camera-standby.png"

    property string gpsIndicatorIcon: "qrc:/images/cameraplus-icon-m-camera-location.png"
    property string faceDetectionIndicatorIcon: "qrc:/images/cameraplus-icon-m-camera-face-detection-screen.png"

    property string recordingDurationIcon: "qrc:/images/cameraplus-icon-m-camera-ongoing-recording.png"
    property string cameraTorchOnIconId: "qrc:/images/cameraplus-icon-m-camera-torch-on.png"
    property string cameraTorchOffIconId: "qrc:/images/cameraplus-icon-m-camera-torch-off.png"
    property string soundMuteOnIconId: "qrc:/images/cameraplus-icon-m-toolbar-volume-off-white-selected.png"
    property string soundMuteOffIconId: "qrc:/images/cameraplus-icon-m-toolbar-volume-white-selected.png"
    property string videoStopIconId: "qrc:/images/cameraplus-icon-m-toolbar-mediacontrol-stop-white.png"
    property string videoPauseIconId: "qrc:/images/cameraplus-icon-m-toolbar-mediacontrol-pause-white.png"
    property string videoPlayIconId: "qrc:/images/cameraplus-icon-m-toolbar-mediacontrol-play-white.png"
    property string cameraManualExposureIconId: "qrc:/images/cameraplus-icon-m-camera-manual-exposure.png"
    property string captureButtonImageIconId: "qrc:/images/cameraplus-icon-m-camera-shutter.png"
    property string captureButtonVideoIconId: "qrc:/images/cameraplus-icon-m-camera-video-record.png"
    property string captureButtonStopRecordingIconId: "qrc:/images/cameraplus-icon-m-camera-video-record-stop.png"
    property string cameraToolBarMenuIcon: "qrc:/images/cameraplus-icon-m-toolbar-settings.png"
    property string deleteIconId: "qrc:/images/cameraplus-icon-m-toolbar-delete-white.png"
    property string shareIconId: "qrc:/images/cameraplus-icon-m-toolbar-share-white.png"
    property string galleryIconId: "qrc:/images/cameraplus-icon-m-camera-roll.png"

    property string flashAutoIconId: "qrc:/images/cameraplus-icon-m-camera-flash-auto.png"
    property string flashOnIconId: "qrc:/images/cameraplus-icon-m-camera-flash-always.png"
    property string flashOffIconId: "qrc:/images/cameraplus-icon-m-camera-flash-off.png"
    property string flashRedEyeIconId: "qrc:/images/cameraplus-icon-m-camera-flash-red-eye.png"

    property string whiteBalanceAutoIconId: "qrc:/images/cameraplus-icon-m-camera-whitebalance-auto.png"
    property string whiteBalanceSunnyIconId: "qrc:/images/cameraplus-icon-m-camera-whitebalance-sunny.png"
    property string whiteBalanceCloudyIconId: "qrc:/images/cameraplus-icon-m-camera-whitebalance-cloudy.png"
    property string whiteBalanceFlourescentIconId: "qrc:/images/cameraplus-icon-m-camera-whitebalance-fluorescent.png"
    property string whiteBalanceTungstenIconId: "qrc:/images/cameraplus-icon-m-camera-whitebalance-tungsten.png"

    property string colorFilterNormalIconId: "qrc:/images/cameraplus-icon-m-camera-no-filter.png"
    property string colorFilterGrayScaleIconId: "qrc:/images/cameraplus-icon-m-camera-filter-black-white.png"
    property string colorFilterSepiaIconId: "qrc:/images/cameraplus-icon-m-camera-filter-sepia.png"
    property string colorFilterVividIconId: "qrc:/images/cameraplus-icon-m-camera-filter-vivid.png"
    property string colorFilterNegativeIconId: "qrc:/images/cameraplus-icon-m-camera-filter-negative.png"
    property string colorFilterSolarizeIconId: "qrc:/images/cameraplus-icon-m-camera-filter-solarize.png"

    property string imageSceneModeAutoIconId: "qrc:/images/cameraplus-icon-m-camera-scene-auto.png"
    property string imageSceneModeCloseupIconId: "qrc:/images/cameraplus-icon-m-camera-scene-macro.png"
    property string imageSceneModeLandscapeIconId: "qrc:/images/cameraplus-icon-m-camera-scene-landscape.png"
    property string imageSceneModePortraitIconId: "qrc:/images/cameraplus-icon-m-camera-scene-portrait.png"
    property string imageSceneModeNightIconId: "qrc:/images/cameraplus-icon-m-camera-night.png"
    property string imageSceneModeSportIconId: "qrc:/images/cameraplus-icon-m-camera-scene-sports.png"

    property string videoSceneModeAutoIconId: "qrc:/images/cameraplus-icon-m-camera-scene-auto.png"
    property string videoSceneModeNightIconId: "qrc:/images/cameraplus-icon-m-camera-video-night.png"

    property string isoAudoIconId: "qrc:/images/cameraplus-icon-m-camera-iso-auto.png"
    property string iso100IconId: "qrc:/images/cameraplus-icon-m-camera-iso-100.png"
    property string iso200IconId: "qrc:/images/cameraplus-icon-m-camera-iso-200.png"
    property string iso400IconId: "qrc:/images/cameraplus-icon-m-camera-iso-400.png"
    property string iso800IconId: "qrc:/images/cameraplus-icon-m-camera-iso-800.png"
    property string resetIconId: "qrc:/images/cameraplus-icon-m-settings-reset.png"
    property string primaryCameraIconId: "qrc:/images/cameraplus-icon-m-camera-switch-to-back.png"
    property string secondaryCameraIconId: "qrc:/images/cameraplus-icon-m-camera-switch-to-front.png"
    property string modeButtonIconId: "qrc:/images/cameraplus-icon-m-mode-switch.png"
    property string selfTimerIconId: "qrc:/images/cameraplus-icon-m-camera-self-timer.png"

    function whiteBalanceIcon(val) {
        switch (val) {
            case WhiteBalance.Auto:
                return whiteBalanceAutoIconId
            case WhiteBalance.Daylight:
                return whiteBalanceSunnyIconId
            case WhiteBalance.Cloudy:
                return whiteBalanceCloudyIconId
            case WhiteBalance.Flourescent:
                return whiteBalanceFlourescentIconId
            case WhiteBalance.Tungsten:
                return whiteBalanceTungstenIconId
            default:
                return ""
        }
    }

    function colorFilterIcon(val) {
        switch (val) {
            case ColorTone.Normal:
                return colorFilterNormalIconId
            case ColorTone.GrayScale:
                return colorFilterGrayScaleIconId
            case ColorTone.Sepia:
                return colorFilterSepiaIconId
            case ColorTone.Vivid:
                return colorFilterVividIconId
            case ColorTone.Negative:
                return colorFilterNegativeIconId
            case ColorTone.Solarize:
                return colorFilterSolarizeIconId
            default:
                return ""
        }
    }

    function imageSceneModeIcon(val) {
        switch (val) {
            case Scene.Auto:
                return imageSceneModeAutoIconId
            case Scene.Closeup:
                return imageSceneModeCloseupIconId
            case Scene.Landscape:
                return imageSceneModeLandscapeIconId
            case Scene.Portrait:
                return imageSceneModePortraitIconId
            case Scene.Night:
                return imageSceneModeNightIconId
            case Scene.Sport:
                return imageSceneModeSportIconId
            default:
                return ""
        }
    }

    function videoSceneModeIcon(val) {
        switch (val) {
            case Scene.Auto:
                return videoSceneModeAutoIconId
            case Scene.Night:
                return videoSceneModeNightIconId
            default:
                return ""
        }
    }

    function flashIcon(val) {
        switch (val) {
            case Flash.Auto:
                return flashAutoIconId
            case Flash.On:
                return flashOnIconId
            case Flash.Off:
                return flashOffIconId
            case Flash.RedEye:
                return flashRedEyeIconId
            default:
                return ""
        }
    }

    function isoIcon(val) {
        switch (val) {
            case 0:
                return isoAudoIconId
            case 100:
                return iso100IconId
            case 200:
                return iso200IconId
            case 400:
                return iso400IconId
            case 800:
                return iso800IconId
            default:
                return ""
        }
    }
}
