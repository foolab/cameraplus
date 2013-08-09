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
import QtCamera 1.0

QtObject {
    property string standbyIcon: "image://theme/cameraplus-icon-l-camera-standby"

    property string gpsIndicatorIcon: "image://theme/cameraplus-icon-m-camera-location"
    property string faceDetectionIndicatorIcon: "image://theme/cameraplus-icon-m-camera-face-detection-screen"

    property string recordingDurationIcon: "image://theme/cameraplus-icon-m-camera-ongoing-recording"
    property string cameraTorchOnIconId: "image://theme/cameraplus-icon-m-camera-torch-on"
    property string cameraTorchOffIconId: "image://theme/cameraplus-icon-m-camera-torch-off"
    property string soundMuteOnIconId: "image://theme/cameraplus-icon-m-toolbar-volume-off-white-selected"
    property string soundMuteOffIconId: "image://theme/cameraplus-icon-m-toolbar-volume-white-selected"
    property string videoStopIconId: "image://theme/cameraplus-icon-m-toolbar-mediacontrol-stop-white"
    property string videoPauseIconId: "image://theme/cameraplus-icon-m-toolbar-mediacontrol-pause-white"
    property string videoPlayIconId: "image://theme/cameraplus-icon-m-toolbar-mediacontrol-play-white"
    property string cameraManualExposureIconId: "image://theme/cameraplus-icon-m-camera-manual-exposure"
    property string cameraVideoIconId: "image://theme/cameraplus-icon-m-camera-video"
    property string cameraImageIconId: "image://theme/cameraplus-icon-m-viewfinder-camera"
    property string captureButtonImageIconId: "image://theme/cameraplus-icon-m-camera-shutter"
    property string captureButtonVideoIconId: "image://theme/cameraplus-icon-m-camera-video-record"
    property string captureButtonRecordingIconId: "image://theme/cameraplus-icon-m-camera-video-record"
    property string cameraToolBarMenuIcon: "image://theme/cameraplus-icon-m-toolbar-back-white"
    property string deleteIconId: "image://theme/cameraplus-icon-m-toolbar-delete-white"
    property string shareIconId: "image://theme/cameraplus-icon-m-toolbar-share-white"
    property string favoriteMarkIconId: "image://theme/cameraplus-icon-m-toolbar-favorite-mark-white"
    property string favoriteUnmarkIconId: "image://theme/cameraplus-icon-m-toolbar-favorite-unmark-white"
    property string galleryIconId: "image://theme/cameraplus-icon-m-camera-roll"

    property string flashAutoIconId: "image://theme/cameraplus-icon-m-camera-flash-auto"
    property string flashOnIconId: "image://theme/cameraplus-icon-m-camera-flash-always"
    property string flashOffIconId: "image://theme/cameraplus-icon-m-camera-flash-off"
    property string flashRedEyeIconId: "image://theme/cameraplus-icon-m-camera-flash-red-eye"

    property string whiteBalanceAutoIconId: "image://theme/cameraplus-icon-m-camera-whitebalance-auto"
    property string whiteBalanceSunsetIconId: "image://theme/cameraplus-icon-m-camera-whitebalance-sunny"
    property string whiteBalanceCloudyIconId: "image://theme/cameraplus-icon-m-camera-whitebalance-cloudy"
    property string whiteBalanceFlourescentIconId: "image://theme/cameraplus-icon-m-camera-whitebalance-fluorescent"
    property string whiteBalanceTungstenIconId: "image://theme/cameraplus-icon-m-camera-whitebalance-tungsten"

    property string colorFilterNormalIconId: "image://theme/cameraplus-icon-m-camera-no-filter"
    property string colorFilterGrayScaleIconId: "image://theme/cameraplus-icon-m-camera-filter-black-white"
    property string colorFilterSepiaIconId: "image://theme/cameraplus-icon-m-camera-filter-sepia"
    property string colorFilterVividIconId: "image://theme/cameraplus-icon-m-camera-filter-vivid"
    property string colorFilterNegativeIconId: "image://theme/cameraplus-icon-m-camera-filter-negative"
    property string colorFilterSolarizeIconId: "image://theme/cameraplus-icon-m-camera-filter-solarize"

    property string imageSceneModeAutoIconId: "image://theme/cameraplus-icon-m-camera-scene-auto"
    property string imageSceneModeCloseupIconId: "image://theme/cameraplus-icon-m-camera-scene-macro"
    property string imageSceneModeLandscapeIconId: "image://theme/cameraplus-icon-m-camera-scene-landscape"
    property string imageSceneModePortraitIconId: "image://theme/cameraplus-icon-m-camera-scene-portrait"
    property string imageSceneModeNightIconId: "image://theme/cameraplus-icon-m-camera-night"
    property string imageSceneModeSportIconId: "image://theme/cameraplus-icon-m-camera-scene-sports"

    property string videoSceneModeAutoIconId: "image://theme/cameraplus-icon-m-camera-scene-auto"
    property string videoSceneModeNightIconId: "image://theme/cameraplus-icon-m-camera-video-night"

    property string isoAudoIconId: "image://theme/cameraplus-icon-m-camera-iso-auto"
    property string iso100IconId: "image://theme/cameraplus-icon-m-camera-iso-100"
    property string iso200IconId: "image://theme/cameraplus-icon-m-camera-iso-200"
    property string iso400IconId: "image://theme/cameraplus-icon-m-camera-iso-400"
    property string iso800IconId: "image://theme/cameraplus-icon-m-camera-iso-800"
    property string resetIconId: "image://theme/cameraplus-icon-m-settings-reset"

    function videoIcon(ratio, res, device) {
        if (res == "high") {
            return "image://theme/cameraplus-icon-m-camera-video-high-resolution"
        } else if (res == "medium") {
            return "image://theme/cameraplus-icon-m-camera-video-fine-resolution"
        } else if (res == "low") {
            return "image://theme/cameraplus-icon-m-camera-video-low-resolution"
        } else {
            return ""
        }
    }

    function whiteBalanceIcon(val) {
        switch (val) {
            case WhiteBalance.Auto:
                return whiteBalanceAutoIconId
            case WhiteBalance.Sunset:
                return whiteBalanceSunsetIconId
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
