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

Item {
    property alias text: label.text
    property alias minimumValue: slider.minimumValue
    property alias maximumValue: slider.maximumValue
    property alias stepSize: slider.stepSize
    property alias value: slider.value
    property alias pressed: slider.pressed
    property alias valueIndicatorVisible: slider.valueIndicatorVisible
    property alias valueIndicatorText: slider.valueIndicatorText
    property bool __showSlider: false

    width: 80
    height: 80

    Rectangle {
        id: rect
        anchors.fill: parent
        color: mouse.pressed ? cameraStyle.pressedColor : "black"
        opacity: 0.5
        radius: 40
        border.width: 1
        border.color: "gray"

        MouseArea {
            id: mouse
            anchors.fill: parent
            onClicked: __showSlider = !__showSlider
        }

        CameraLabel {
            id: label
            anchors.centerIn: parent
            font.pixelSize: 36
            font.bold: true
        }
    }

    CameraSlider {
        id: slider
        anchors {
            verticalCenter: rect.verticalCenter
            left: settings.leftHandedMode ? undefined : rect.right
            leftMargin: 20
            right : settings.leftHandedMode ? rect.left : undefined
            rightMargin: 20
        }
        visible: opacity > 0.0
        opacity: rootWindow.active && __showSlider ? 1.0 : 0.0
        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }
    }

    Timer {
        interval: 2000
        running: slider.visible && !slider.pressed
        onTriggered: parent.__showSlider = false
    }

    function close() {
        __showSlider = false
    }
}
