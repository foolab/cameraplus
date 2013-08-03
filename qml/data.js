// -*- js -*-

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

// Shared between all QML components
//.pragma library

// White Balance
var __wb = [
    [WhiteBalance.Auto, "icon-m-camera-whitebalance-auto"],
    [WhiteBalance.Sunset, "icon-m-camera-whitebalance-sunny"],
    [WhiteBalance.Cloudy, "icon-m-camera-whitebalance-cloudy"],
    [WhiteBalance.Flourescent, "icon-m-camera-whitebalance-fluorescent"],
    [WhiteBalance.Tungsten, "icon-m-camera-whitebalance-tungsten"],
];

// Color Filter
var __cf = [
    [ColorTone.Normal, "icon-m-camera-no-filter"],
    [ColorTone.GrayScale, "icon-m-camera-filter-black-white"],
    [ColorTone.Sepia, "icon-m-camera-filter-sepia"],
    [ColorTone.Vivid, "icon-m-camera-filter-vivid"],
    [ColorTone.Negative, "icon-m-camera-filter-negative"],
    [ColorTone.Solarize, "icon-m-camera-filter-solarize"]
];

// Image Scene Mode
var __ism = [
    [Scene.Auto, "icon-m-camera-scene-auto"],
    [Scene.Closeup, "icon-m-camera-scene-macro"],
    [Scene.Landscape, "icon-m-camera-scene-landscape"],
    [Scene.Portrait, "icon-m-camera-scene-portrait"],
    [Scene.Night, "icon-m-camera-night"],
    [Scene.Sport, "icon-m-camera-scene-sports"]
];

// Video Scene Mode
var __vsm = [
    [Scene.Auto, "icon-m-camera-scene-auto"],
    [Scene.Night, "icon-m-camera-video-night"]
];

var __flash = [
    [Flash.Auto, "icon-m-camera-flash-auto"],
    [Flash.On, "icon-m-camera-flash-always"],
    [Flash.Off, "icon-m-camera-flash-off"],
    [Flash.RedEye, "icon-m-camera-flash-red-eye"]
];

// ISO
var __iso = [
    [0, "icon-m-camera-iso-auto"],
    [100, "icon-m-camera-iso-100"],
    [200, "icon-m-camera-iso-200"],
    [400, "icon-m-camera-iso-400"],
    [800, "icon-m-camera-iso-800"]
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

function wbIcon(wb) {
    return filterData(wb, __wb, 1);
}

function cfIcon(cf) {
    return filterData(cf, __cf, 1);
}

function ismIcon(sm) {
    return filterData(sm, __ism, 1);
}

function vsmIcon(sm) {
    return filterData(sm, __vsm, 1);
}

function isoIcon(value) {
    return filterData(value, __iso, 1);
}

function flashIcon(val) {
    return filterData(val, __flash, 1);
}
