// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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

CameraPage {
        id: page

        controlsVisible: false
        policyMode: CameraResources.PostCapture
        needsPipeline: false
        standbyVisible: false

        property alias source: video.source
        function play() {
                video.play();
        }

        MouseArea {
                anchors.fill: parent
                onClicked: toolBar.visible = !toolBar.visible
        }

		Video {
                id: video
                anchors.fill: parent
                onStopped: pageStack.pop(undefined, true);
		}

        Connections {
                target: Qt.application
                onActiveChanged: {
                        if (!Qt.application.active) {
                                video.stop();
                        }
                }
        }

        ToolBar {
                id: toolBar
                opacity: 0.8
                anchors.bottom: parent.bottom
                tools: ToolBarLayout {
                        id: layout

                        ToolIcon { iconId: "icon-m-toolbar-mediacontrol-stop-white"; onClicked: { video.stop(); } }

                        Slider {
                                id: slider
                                height: toolBar.height

                                platformStyle: SliderStyle {
                                        // HACK
                                        handleBackground: " "
                                        handleBackgroundPressed: " "
                                }

                                minimumValue: 0
                                maximumValue: video.duration
                                value: video.position
                                orientation: Qt.Horizontal

                                onPressedChanged: {
                                        if (!slider.pressed) {
                                                video.position = slider.value;
                                        }
                                }
                        }

                        ToolIcon {
                                id: control
                                iconId: !video.paused ? "icon-m-toolbar-mediacontrol-pause-white" : "icon-m-toolbar-mediacontrol-play-white"
                                onClicked: {
                                        if (!video.paused) {
                                                video.pause();
                                        }
                                        else {
                                                video.play();
                                        }
                                }
                        }
                }
        }
}
