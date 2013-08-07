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
    property string standbyIcon: "image://theme/icon-l-camera-standby"

    property string gpsIndicatorIcon: "image://theme/icon-m-camera-location"
    property string faceDetectionIndicatorIcon: "image://theme/icon-m-camera-face-detection-screen"

    property string recordingDurationIcon: "image://theme/icon-m-camera-ongoing-recording"
    property string cameraTorchOnIconId: "icon-m-camera-torch-on"
    property string cameraTorchOffIconId: "icon-m-camera-torch-off"
    property string soundMuteOnIconId: "icon-m-toolbar-volume-off-white-selected"
    property string soundMuteOffIconId: "icon-m-toolbar-volume-white-selected"
    property string videoStopIconId: "icon-m-toolbar-mediacontrol-stop-white"
    property string videoPauseIconId: "icon-m-toolbar-mediacontrol-pause-white"
    property string videoPlayIconId: "icon-m-toolbar-mediacontrol-play-white"
    property string cameraManualExposureIconId: "icon-m-camera-manual-exposure"
    property string cameraVideoIconId: "icon-m-camera-video"
    property string cameraImageIconId: "icon-m-viewfinder-camera"
    property string captureButtonImageIconId: "icon-m-camera-shutter"
    property string captureButtonVideoIconId: "icon-m-camera-video-record"
    property string captureButtonRecordingIconId: "icon-m-camera-video-record"
    property string cameraToolBarMenuIcon: "icon-m-toolbar-back-white"
    property string deleteIconId: "icon-m-toolbar-delete-white"
    property string shareIconId: "icon-m-toolbar-share-white"
    property string favoriteMarkIconId: "icon-m-toolbar-favorite-mark-white"
    property string favoriteUnmarkIconId: "icon-m-toolbar-favorite-unmark-white"
    property string menuIconId: "icon-m-toolbar-view-menu-white"

    property string flashAutoIconId: "icon-m-camera-flash-auto"
    property string flashOnIconId: "icon-m-camera-flash-always"
    property string flashOffIconId: "icon-m-camera-flash-off"
    property string flashRedEyeIconId: "icon-m-camera-flash-red-eye"

    property string whiteBalanceAutoIconId: "icon-m-camera-whitebalance-auto"
    property string whiteBalanceSunsetIconId: "icon-m-camera-whitebalance-sunny"
    property string whiteBalanceCloudyIconId: "icon-m-camera-whitebalance-cloudy"
    property string whiteBalanceFlourescentIconId: "icon-m-camera-whitebalance-fluorescent"
    property string whiteBalanceTungstenIconId: "icon-m-camera-whitebalance-tungsten"

    property string colorFilterNormalIconId: "icon-m-camera-no-filter"
    property string colorFilterGrayScaleIconId: "icon-m-camera-filter-black-white"
    property string colorFilterSepiaIconId: "icon-m-camera-filter-sepia"
    property string colorFilterVividIconId: "icon-m-camera-filter-vivid"
    property string colorFilterNegativeIconId: "icon-m-camera-filter-negative"
    property string colorFilterSolarizeIconId: "icon-m-camera-filter-solarize"

    property string imageSceneModeAutoIconId: "icon-m-camera-scene-auto"
    property string imageSceneModeCloseupIconId: "icon-m-camera-scene-macro"
    property string imageSceneModeLandscapeIconId: "icon-m-camera-scene-landscape"
    property string imageSceneModePortraitIconId: "icon-m-camera-scene-portrait"
    property string imageSceneModeNightIconId: "icon-m-camera-night"
    property string imageSceneModeSportIconId: "icon-m-camera-scene-sports"

    property string videoSceneModeAutoIconId: "icon-m-camera-scene-auto"
    property string videoSceneModeNightIconId: "icon-m-camera-video-night"

    property string isoAudoIconId: "icon-m-camera-iso-auto"
    property string iso100IconId: "icon-m-camera-iso-100"
    property string iso200IconId: "icon-m-camera-iso-200"
    property string iso400IconId: "icon-m-camera-iso-400"
    property string iso800IconId: "icon-m-camera-iso-800"

    function videoIcon(ratio, res, device) {
        if (res == "high") {
            return "icon-m-camera-video-high-resolution"
        } else if (res == "medium") {
            return "icon-m-camera-video-fine-resolution"
        } else if (res == "low") {
            return "icon-m-camera-video-low-resolution"
        } else {
            return ""
        }
    }

    property variant __imageData: [
        ["3:2", "low", "icon-m-camera-resolution-3m"],
        ["3:2", "medium", "icon-m-camera-resolution-6m"],
        ["3:2", "high", "icon-m-camera-resolution-7m"],
        ["4:3", "low", "icon-m-camera-resolution-3m"],
        ["4:3", "medium", "icon-m-camera-resolution-6m"],
        ["4:3", "high", "icon-m-camera-resolution-8m"],
        ["16:9", "low", "icon-m-camera-resolution-3m"],
        ["16:9", "medium", "icon-m-camera-resolution-6m"],
        ["16:9", "high", "icon-m-camera-resolution-7m"]
    ]

    function imageIcon(ratio, res, device) {
        var x = 0
        var len = __imageData.length
        for (x = 0; x < len; x++) {
            if (__imageData[x][0] == ratio && __imageData[x][1] == res) {
                return __imageData[x][2]
            }
        }

        return ""
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

    function flashIndicatorIcon(val) {
        return flashIcon(val) + "-screen"
    }

    function whiteBalanceIndicatorIcon(val) {
        if (val == WhiteBalance.Auto) {
            return ""
        } else {
            return whiteBalanceIcon(val) + "-screen"
        }
    }

    function colorFilterIndicatorIcon(val) {
        if (val == ColorTone.Normal) {
            return ""
        } else {
            return colorFilterIcon(val) + "-screen"
        }
    }

    function imageSceneModeIndicatorIcon(val) {
        if (val == Scene.Auto) {
            return ""
        } else {
            return imageSceneModeIcon(val) + "-screen"
        }
    }

    function videoSceneModeIndicatorIcon(val) {
        if (val == Scene.Auto) {
            return ""
        } else {
            return videoSceneModeIcon(val) + "-screen"
        }
    }
}
