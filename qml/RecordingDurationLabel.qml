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

Rectangle {
    property int duration

    anchors.bottom: parent.bottom
    anchors.right: parent.right
    anchors.rightMargin: 20
    anchors.bottomMargin: 20

    color: "black"
    opacity: 0.5
    width: 100
    height: 30

    Image {
        id: recordingIcon
        source: "image://theme/icon-m-camera-ongoing-recording"
        width: 20
        height: 20
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 5
        sourceSize.width: 20
        sourceSize.height: 20
    }

    CameraLabel {
        function formatDuration(dur) {
            var secs = parseInt(recordingDuration.duration)
            var minutes = Math.floor(secs / 60)
            var seconds = secs - (minutes * 60)

            var date = new Date()
            date.setSeconds(seconds)
            date.setMinutes(minutes)
            return Qt.formatTime(date, "mm:ss")
        }

        text: formatDuration(parent.duration)
        anchors.left: recordingIcon.right
        anchors.leftMargin: 5
    }
}
