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

Image {
    id: image
    x: 0
    y: 0
    width: parent.width
    height: parent.height
    property alias animationRunning: animation.running

    cache: false
    fillMode: Image.PreserveAspectFit

    MouseArea {
        anchors.fill: parent
        enabled: parent.visible
    }

    SequentialAnimation {
        id: animation

        onRunningChanged: {
            if (!running) {
                image.visible = false
                image.x = 0
            }
        }

        PauseAnimation {
            duration: 250
            alwaysRunToEnd: true
        }

        NumberAnimation {
            target: image
            property: "x"
            from: 0
            to: parent.width
            duration: 250
            alwaysRunToEnd: true
        }
    }

    function setPreview(preview) {
        image.source = preview
        image.x = 0
        image.visible = true
        animation.start()
    }
}
