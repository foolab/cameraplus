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
import CameraPlus 1.0
import QtCamera 1.0
import QtCameraExtras 1.0

Item {
    id: page

    signal finished
    property alias source: video.source

    function play() {
        return video.play()
    }

    function stop() {
        return video.stop()
    }

    MouseArea {
        anchors.top: parent.top
        anchors.bottom: toolBar.top
        anchors.left: parent.left
        anchors.right: parent.right

        onClicked: toolBar.show = !toolBar.show
    }

    Timer {
        id: hideTimer
        running: toolBar.show
        interval: 3000
        onTriggered: toolBar.show = false
    }

    VideoPlayer {
        id: video
        anchors.fill: parent
        cameraConfig: camera.cameraConfig

        onError: showError(qsTr("Error playing video. Please try again or restart the application"))

        function toggle() {
            if (!video.paused) {
                video.pause()
            } else {
                page.play()
            }
        }

        onStateChanged: {
            if (state == VideoPlayer.StateStopped) {
                page.finished()
            }
        }
    }

    Connections {
        target: Qt.application
        onActiveChanged: {
            if (!Qt.application.active) {
                video.stop()
            }
        }
    }

    CameraToolBar {
        id: toolBar

        property bool show: true

        hideBack: true
        expanded: true
        anchors.bottom: parent.bottom
        anchors.bottomMargin: show ? 20 : -1 * (height + 20)
        anchors.left: parent.left
        anchors.leftMargin: 20
        opacity: 0.5

        Behavior on anchors.bottomMargin {
            PropertyAnimation { duration: 200; }
        }

        items: [
            CameraToolIcon {
                iconId: cameraTheme.videoStopIconId
                onClicked: video.stop()
            },
            CameraSlider {
                id: slider
                height: toolBar.height
                anchors.verticalCenter: parent.verticalCenter

                handleBackground: ""
                handleBackgroundPressed: ""

                minimumValue: 0
                maximumValue: video.duration
                value: video.position
                orientation: Qt.Horizontal

                onPressedChanged: {
                    if (!slider.pressed) {
                        video.position = slider.value
                    }

                    hideTimer.restart()
                }
            },
            CameraToolIcon {
                id: control
                iconId: !video.paused ? cameraTheme.videoPauseIconId : cameraTheme.videoPlayIconId
                onClicked: {
                    video.toggle()
                    hideTimer.restart()
                }
            }
        ]
    }
}
