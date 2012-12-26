// -*- js -*-

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

// Shared between all QML components
//.pragma library

// White Balance
var __wb = [
    [WhiteBalance.Auto, qsTr("Automatic"),
     "icon-m-camera-whitebalance-auto-selected", "icon-m-camera-whitebalance-auto"],
    [WhiteBalance.Sunset, qsTr("Sunny"),
     "icon-m-camera-whitebalance-sunny-selected", "icon-m-camera-whitebalance-sunny"],
    [WhiteBalance.Cloudy, qsTr("Cloudy"),
     "icon-m-camera-whitebalance-cloudy-selected", "icon-m-camera-whitebalance-cloudy"],
    [WhiteBalance.Flourescent, qsTr("Flourescent"),
     "icon-m-camera-whitebalance-fluorescent-selected", "icon-m-camera-whitebalance-fluorescent"],
    [WhiteBalance.Tungsten, qsTr("Tungsten"),
     "icon-m-camera-whitebalance-tungsten-selected", "icon-m-camera-whitebalance-tungsten"],
];

// Color Filter
var __cf = [
    [ColorTone.Normal, qsTr("Off"),
     "icon-m-camera-no-filter-selected", "icon-m-camera-no-filter"],
    [ColorTone.GrayScale, qsTr("Black & white"),
     "icon-m-camera-filter-black-white-selected", "icon-m-camera-filter-black-white"],
    [ColorTone.Sepia, qsTr("Sepia"),
     "icon-m-camera-filter-sepia-selected", "icon-m-camera-filter-sepia"],
    [ColorTone.Vivid, qsTr("Vivid"),
     "icon-m-camera-filter-vivid-selected", "icon-m-camera-filter-vivid"],
    [ColorTone.Negative, qsTr("Negative"),
     "icon-m-camera-filter-negative-selected", "icon-m-camera-filter-negative"],
    [ColorTone.Solarize, qsTr("Solarize"),
     "icon-m-camera-filter-solarize-selected", "icon-m-camera-filter-solarize"]
];

// Image Scene Mode
var __ism = [
    [Scene.Auto, qsTr("Automatic"),
     "icon-m-camera-scene-auto-selected", "icon-m-camera-scene-auto"],
    [Scene.Closeup, qsTr("Macro"),
     "icon-m-camera-scene-macro-selected", "icon-m-camera-scene-macro"],
    [Scene.Landscape, qsTr("Landscape"),
     "icon-m-camera-scene-landscape-selected", "icon-m-camera-scene-landscape"],
    [Scene.Portrait, qsTr("Portrait"),
     "icon-m-camera-scene-portrait-selected", "icon-m-camera-scene-portrait"],
    [Scene.Night, qsTr("Night"),
     "icon-m-camera-night-selected", "icon-m-camera-night"],
    [Scene.Sport, qsTr("Sports"),
     "icon-m-camera-scene-sports-selected", "icon-m-camera-scene-sports"]
];

// Video Scene Mode
var __vsm = [
    [Scene.Auto, qsTr("Automatic"),
     "icon-m-camera-scene-auto-selected", "icon-m-camera-scene-auto"],
    [Scene.Night, qsTr("Video at night"),
     "icon-m-camera-video-night-selected", "icon-m-camera-video-night"]
];

var __flash = [
    [Flash.Auto,"icon-m-camera-flash-auto-selected", "icon-m-camera-flash-auto"],
    [Flash.On, "icon-m-camera-flash-always-selected", "icon-m-camera-flash-always"],
    [Flash.Off, "icon-m-camera-flash-off-selected", "icon-m-camera-flash-off"],
    [Flash.RedEye, "icon-m-camera-flash-red-eye-selected", "icon-m-camera-flash-red-eye"]
];

// ISO
var __iso = [
    [0, "icon-m-camera-iso-auto"],
    [100, "icon-m-camera-iso-100"],
    [200, "icon-m-camera-iso-200"],
    [400, "icon-m-camera-iso-400"],
    [800, "icon-m-camera-iso-800"]
];

// Image resolutions
var __image = [
    ["3:2", "low", "icon-m-camera-resolution-3m"],
    ["3:2", "medium", "icon-m-camera-resolution-6m"],
    ["3:2", "high", "icon-m-camera-resolution-7m"],
    ["4:3", "low", "icon-m-camera-resolution-3m"],
    ["4:3", "medium", "icon-m-camera-resolution-6m"],
    ["4:3", "high", "icon-m-camera-resolution-8m"],
    ["16:9", "low", "icon-m-camera-resolution-3m"],
    ["16:9", "medium", "icon-m-camera-resolution-6m"],
    ["16:9", "high", "icon-m-camera-resolution-7m"]
];

// Video resolutions
var __video = [
    ["low", "icon-m-camera-video-low-resolution"],
    ["medium", "icon-m-camera-video-fine-resolution"],
    ["high", "icon-m-camera-video-high-resolution"],
];

function filterData(val, data, item) {
    var x = 0;
    var i = data.length;
    for (x = 0; x < i; x++) {
        if (data[x][0] == val) {
            return data[x][item];
        }
    }
}

function wbSelectedIcon(wb) {
    return filterData(wb, __wb, 2);
}

function wbIcon(wb) {
    return filterData(wb, __wb, 3);
}

function wbName(wb) {
    return filterData(wb, __wb, 1);
}

function cfSelectedIcon(cf) {
    return filterData(cf, __cf, 2);
}

function cfIcon(cf) {
    return filterData(cf, __cf, 3);
}

function cfName(cf) {
    return filterData(cf, __cf, 1);
}

function ismIcon(sm) {
    return filterData(sm, __ism, 3);
}

function ismSelectedIcon(sm) {
    return filterData(sm, __ism, 2);
}

function ismName(sm) {
    return filterData(sm, __ism, 1);
}

function vsmIcon(sm) {
    return filterData(sm, __vsm, 3);
}

function vsmSelectedIcon(sm) {
    return filterData(sm, __vsm, 2);
}

function vsmName(sm) {
    return filterData(sm, __vsm, 1);
}

function isoIcon(value) {
    var x = 0;
    var len = __iso.length;
    for (x = 0; x < len; x++) {
	if (__iso[x][0] == value) {
	    return __iso[x][1];
	}
    }

    return "";
}

function imageIcon(aspect, res) {
    var x = 0;
    var len = __image.length;
    for (x = 0; x < len; x++) {
	if (__image[x][0] == aspect && __image[x][1] == res) {
	    return __image[x][2];
	}
    }

    return "";
}

function videoIcon(res) {
    return filterData(res, __video, 1);
}

function flashIcon(val) {
    return filterData(val, __flash, 2);
}

function flashPressedIcon(val) {
    return filterData(val, __flash, 1);
}
