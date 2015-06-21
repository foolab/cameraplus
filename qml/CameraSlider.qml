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
import CameraPlus 1.0

MouseArea {
    id: mouse

    property alias minimumValue: range.minimumValue
    property alias maximumValue: range.maximumValue
    property alias value: range.value
    property alias stepSize: range.stepSize

    property bool valueIndicatorVisible: mouse.pressed
    property string valueIndicatorText: value.toFixed(1)
    property bool handleVisible: true

    width: parent.width / 2
    height: cameraStyle.sliderHeight

    RangeModel {
        id: range
        minimumValue: 0.0
        maximumValue: 1.0
        value: 0.0
        stepSize: 0.0
        positionAtMinimum: 0
        positionAtMaximum: width - knob.width
        inverted: false
    }

    Rectangle {
        visible: opacity > 0
        color: "white"
        height: parent.height
        width: label.width * 2
        radius: height / 3
        opacity: valueIndicatorVisible ? 1.0 : 0.0

        Behavior on opacity {
            NumberAnimation { duration: visible ? 0 : 500; }
        }

        anchors {
            bottom: parent.top
            horizontalCenter: parent.horizontalCenter
        }

        CameraLabel {
            id: label
            height: parent.height
            text: valueIndicatorText
            anchors.horizontalCenter: parent.horizontalCenter
            color: "black"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

    drag {
        target: knob
        axis: Drag.XAxis
        minimumX: range.positionAtMinimum
        maximumX: range.positionAtMaximum
    }

    // when there is no mouse interaction, the handle's position binds to the value
    Binding {
        when: !mouse.drag.active
        target: knob
        property: "x"
        value: range.position
    }

    // when the slider is dragged, the value binds to the handle's position
    Binding {
        when: mouse.drag.active
        target: range
        property: "position"
        value: knob.x
    }

    property real __oldPos
    onPressed: {
        __oldPos = range.position
        range.position = mouse.x - knob.width / 2
    }

    onCanceled: {
        range.position = __oldPos
    }

    Rectangle {
        id: groove
        anchors {
            left: parent.left
            right: parent.right
            leftMargin: knob.width / 2
            rightMargin: knob.width / 2
            verticalCenter: parent.verticalCenter
        }

        height: parent.height / 4
        color: "gray"
    }

    Rectangle {
        id: fill

        anchors {
            top: groove.top
            left: groove.left
            bottom: groove.bottom
        }

        width: knob.x
        color: cameraStyle.pressedColor
    }

    Rectangle {
        id: knob
        visible: handleVisible
        width: mouse.height
        height: mouse.height
        radius: width / 4
        smooth: true
        anchors.verticalCenter: parent.verticalCenter
        color: "white"
    }
}
