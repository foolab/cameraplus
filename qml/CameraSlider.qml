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

import QtQuick 2.0

MouseArea {
    id: mouse

    property real minimumValue: 0.0
    property real maximumValue: 1.0
    property real value: 0.0
    property bool valueIndicatorVisible: mouse.pressed
    property string valueIndicatorText: value.toFixed(1)
    property bool handleVisible: true

    width: 500
    height: 40

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
        minimumX: 0
        maximumX: width - knob.width
    }

    property bool __hack
    property bool __completed
    property real val: ((knob.x / drag.maximumX) * (maximumValue - minimumValue)) + minimumValue

    onValChanged: {
        if (!__completed) {
            return
        }

        __hack = true
        value = val
        __hack = false
    }

    Component.onCompleted: {
        __completed = true
        setPosition()
    }

    onValueChanged: setPosition()
    onMinimumValueChanged: setPosition()
    onMaximumValueChanged: setPosition()

    function setPosition() {
        if (__hack) {
            return
        }

        var pos = ((value - minimumValue) / (maximumValue - minimumValue)) * drag.maximumX
        Math.max(pos, drag.minimumX)
        Math.min(pos, drag.maximumX)
        knob.x = pos
    }

    property real __oldPos
    onPressed: {
        __oldPos = knob.x
        knob.x = mouse.x - knob.width / 2
    }

    onCanceled: {
        knob.x = __oldPos
    }

    onClicked: knob.x = mouse.x - knob.width / 2

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
