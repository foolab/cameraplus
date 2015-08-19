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
import Sailfish.Silica 1.0

// TODO:
QtObject {
    property color pressedColor: "blue"
    property color borderColor: "gray"
    property color backgroundColor: "black"
    property real zoomSliderStepSize: 1.0
    property real captureButtonWidth: Theme.itemSizeLarge
    property real captureButtonHeight: captureButtonWidth
    property real modeButtonWidth: Theme.itemSizeSmall
    property real modeButtonHeight: modeButtonWidth
    property real onScreenOptionWidth: Theme.itemSizeSmall
    property real onScreenOptionHeight: onScreenOptionWidth
    property real toolIconWidth: Theme.itemSizeExtraSmall
    property real toolIconHeight: toolIconWidth
    property real toolBarHeight: Theme.itemSizeSmall
    property real pluginSelectorDelagateHeight: Theme.itemSizeLarge
    property real padding: Theme.paddingMedium
    property real standByIconWidth: Theme.itemSizeHuge
    property real standByIconHeight: standByIconWidth
    property real indicatorArrayHeight: Theme.iconSizeSmall
    property real spacingSmall: Theme.paddingSmall
    property real spacingMedium: Theme.paddingMedium
    property real spacingLarge: Theme.paddingLarge
    property real fontSizeSmall: Theme.fontSizeSmall
    property real fontSizeMedium: Theme.fontSizeMedium
    property real fontSizeLarge: Theme.fontSizeLarge
    property real sliderHeight: Theme.itemSizeExtraSmall * 0.5
    property real zoomSliderHeight: Theme.itemSizeExtraSmall * 0.75
    property real volumeSliderHeight: Theme.itemSizeExtraSmall / 4
    property real focusReticleNormalWidth: focusReticleNormalHeight * 5 / 3
    property real focusReticleNormalHeight: Theme.itemSizeHuge
    property real focusReticlePressedWidth: focusReticlePressedHeight * 5 / 3
    property real focusReticlePressedHeight: Theme.itemSizeSmall
    property real focusReticleTouchWidth: focusReticleTouchHeight * 5 / 3
    property real focusReticleTouchHeight: Theme.itemSizeLarge
    property real focusReticleCenterRectDimension: Theme.itemSizeMedium
}
