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

import QtQuick 1.1

QtObject {
    property color pressedColor: "#0080FF"
    property color borderColor: "gray"
    property color backgroundColor: "black"
    property real zoomSliderStepSize: 0.1
    property real captureButtonWidth: 100
    property real captureButtonHeight: captureButtonWidth
    property real modeButtonWidth: 75
    property real modeButtonHeight: modeButtonWidth
    property real onScreenOptionWidth: 80
    property real onScreenOptionHeight: 80
    property real toolIconWidth: 64
    property real toolIconHeight: 64
    property real toolBarHeight: 74
    property real pluginSelectorDelagateHeight: 100
    property real padding: 20
    property real standByIconWidth: 200
    property real standByIconHeight: 200
    property real indicatorArrayHeight: 32
    property real spacingSmall: 5
    property real spacingMedium: 10
    property real spacingLarge: 20
    property real fontSizeSmall: 24
    property real fontSizeMedium: 32
    property real fontSizeLarge: 36
    property real sliderHeight: 40
    property real zoomSliderHeight: sliderHeight
    property real volumeSliderHeight: 25
    property real focusReticleNormalWidth: 250
    property real focusReticleNormalHeight: 150
    property real focusReticlePressedWidth: 150
    property real focusReticlePressedHeight: 90
    property real focusReticleTouchWidth: 200
    property real focusReticleTouchHeight: 120
    property real focusReticleCenterRectDimension: 100
}
