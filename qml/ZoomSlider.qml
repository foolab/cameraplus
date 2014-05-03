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

CameraSlider {
    id: slider
    property Camera camera

    Binding {
        target: camera ? camera.zoom : null
        property: "value"
        value: slider.value
    }

    Connections {
        target: camera
        onModeChanged: slider.value = camera.zoom.minimum;
    }

    Connections {
        target: keys

        onVolumeUpPressed: {
            if (settings.zoomAsShutter) {
                return;
            }

            slider.value = Math.min(slider.value + slider.stepSize, slider.maximumValue)
            hackTimer.running = true
        }

        onVolumeDownPressed: {
            if (settings.zoomAsShutter) {
                return;
            }

            slider.value = Math.max(slider.value - slider.stepSize, slider.minimumValue)
            hackTimer.running = true
        }
    }

    width: 500
    stepSize: 0.1
    height: 40
    value: camera ? camera.zoom.value : 0
    minimumValue: camera ? camera.zoom.minimum : 0
    maximumValue: camera ? camera.zoom.maximum : 0
    valueIndicatorVisible: false
    handleVisible: false
    anchors.top: parent.top
    anchors.topMargin: 0
    anchors.horizontalCenter: parent.horizontalCenter

    state: "hidden"
    states: [
        State {
            name: "visible"
            when: slider.pressed || hackTimer.running
            PropertyChanges { target: slider; opacity: 1.0 }
        },
        State {
            name: "hidden"
            when: !slider.pressed
            PropertyChanges { target: slider; opacity: 0.2 }
        }
    ]

    transitions: Transition {
        to: "hidden"
            SequentialAnimation {
                PauseAnimation { duration: 2000 }
                NumberAnimation { target: slider; property: "opacity"; duration: 250 }
            }
    }

    Timer {
        id: hackTimer
        interval: 1
    }
}
