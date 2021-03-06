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

    DeviceKeys {
        id: zoomKeys
        active: rootWindow.active && pipelineManager.scaleAcquired
        repeat: true

        onVolumeUpPressed: {
            timer.restart()
            video.volume = Math.min(video.volume + 2, 100)
        }

        onVolumeDownPressed: {
            timer.restart()
            video.volume = Math.max(video.volume - 2, 0)
        }
    }

    VideoPlayer {
        CameraSlider {
            id: volumeControl
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            minimumValue: 0
            maximumValue: width
            value: (width * video.volume) / 100
            handleVisible: false
            valueIndicatorVisible: false
            height: cameraStyle.volumeSliderHeight
            opacity: timer.running ? 1.0 : 0.0

            Behavior on opacity {
                NumberAnimation { duration: 250 }
            }

            Timer {
                id: timer
                interval: 500
                repeat: false
            }
        }

        id: video
        anchors.fill: parent
        cameraConfig: camera.cameraConfig
        onError: showError(qsTr("Error playing video. Please try again or restart the application"))

        function toggle() {
            if (state != VideoPlayer.StatePaused) {
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
        target: rootWindow
        onActiveChanged: {
            if (!rootWindow.active) {
                video.stop()
            }
        }
    }

    CameraToolBar {
        id: toolBar

        property bool show: true

        hideBack: true
        expanded: true
        anchors {
            bottom: parent.bottom
            bottomMargin: cameraStyle.padding
            left: parent.left
            leftMargin: cameraStyle.padding
        }
        width: parent.width - 2 * anchors.leftMargin
        opacity: show ? 0.8 : 0.0
        visible: opacity > 0

        Behavior on opacity {
            PropertyAnimation { duration: 200; }
        }

        tools: CameraToolBarTools {
            CameraToolIcon {
                iconSource: cameraTheme.videoStopIconId
                onClicked: video.stop()
            }

            CameraSlider {
                id: slider
                width: toolBar.width / 2
                anchors.verticalCenter: parent.verticalCenter
                handleVisible: false
                minimumValue: 0
                maximumValue: video.duration
                value: video.position
                onPressedChanged: {
                    if (!slider.pressed) {
                        video.position = slider.value
                    }

                    hideTimer.restart()
                }
            }

            CameraToolIcon {
                id: control
                iconSource: video.state != VideoPlayer.StatePaused ? cameraTheme.videoPauseIconId : cameraTheme.videoPlayIconId
                onClicked: {
                    video.toggle()
                    hideTimer.restart()
                }
            }
        }
    }
}
