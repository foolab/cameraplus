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

import QtQuick 1.1
import com.nokia.meego 1.1
import QtMultimediaKit 1.1
import CameraPlus 1.0

// TODO: error reporting

CameraPage {
        id: page

        property bool popTwice: false
        controlsVisible: false
        policyMode: CameraResources.None
        activationData: ControlsActivationData {standbyVisible: false}

        property alias source: video.source

        function play() {
                video.play();
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
                onTriggered: toolBar.show = false;
        }

		Video {
                id: video
                anchors.fill: parent

                function toggle() {
                        if (!video.paused) {
                                video.pause();
                        }
                        else {
                                page.play();
                        }
                }

                onStopped: {
                        source = "";
                        pageStack.pop(undefined, true);

                        if (page.popTwice) {
                                pageStack.pop(undefined);
                        }
                }
		}

        Connections {
                target: Qt.application
                onActiveChanged: {
                        if (!Qt.application.active) {
                                video.stop();
                        }
                }
        }

        CameraToolBar {
                id: toolBar

                property bool show: true

                manualBack: true
                expanded: true
                anchors.bottom: parent.bottom
                anchors.bottomMargin: show ? 20 : -1 * (height + 20)
                anchors.left: parent.left
                anchors.leftMargin: 20
                opacity: 0.5

                Behavior on anchors.bottomMargin {
                        PropertyAnimation { duration: 200; }
                }

                onClicked: {
                        page.popTwice = true;
                        video.stop();
                }

                items: [
                        ToolIcon { iconId: "icon-m-toolbar-mediacontrol-stop-white"; onClicked: { video.stop(); } },
                        Slider {
                                id: slider
                                height: toolBar.height
                                anchors.verticalCenter: parent.verticalCenter

                                platformStyle: SliderStyle {
                                        handleBackground: ""
                                        handleBackgroundPressed: ""
                                }

                                minimumValue: 0
                                maximumValue: video.duration
                                value: video.position
                                orientation: Qt.Horizontal

                                onPressedChanged: {
                                        if (!slider.pressed) {
                                                video.position = slider.value;
                                        }

                                        hideTimer.restart();
                                }
                        },
                        ToolIcon {
                                id: control
                                iconId: !video.paused ? "icon-m-toolbar-mediacontrol-pause-white" : "icon-m-toolbar-mediacontrol-play-white"
                                onClicked: {
                                        video.toggle();
                                        hideTimer.restart();
                                }
                        }
                ]
        }
}
