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
import QtCamera 1.0
import CameraPlus 1.0
import "data.js" as Data

CameraPage {
        id: page

        controlsVisible: false
        policyMode: CameraResources.Video
        needsPipeline: true

        Rectangle {
                color: "black"
                width: parent.width
                height: row.height + title.height + 30
                anchors.bottom: toolBar.top
                opacity: 0.5

                SectionHeader {
                        id: title
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        text: qsTr("White balance");
                }

                Row {
                        id: row
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: 20
                        anchors.rightMargin: 20
                        anchors.top: title.bottom
                        anchors.topMargin: 10

                        IconButton {
                                width: parent.width / 5
                                normalIcon: "image://theme/" + Data.wbIcon(value);
                                checkedIcon: "image://theme/" + Data.wbSelectedIcon(value);
                                value: WhiteBalance.Auto
                                savedValue: settings.videoWhiteBalance
                                onClicked: settings.videoWhiteBalance = value;
                                text: Data.wbName(value);
                        }

                        IconButton {
                                width: parent.width / 5
                                normalIcon: "image://theme/" + Data.wbIcon(value);
                                checkedIcon: "image://theme/" + Data.wbSelectedIcon(value);
                                value: WhiteBalance.Sunset
                                savedValue: settings.videoWhiteBalance
                                onClicked: settings.videoWhiteBalance = value;
                                text: Data.wbName(value);
                        }

                        IconButton {
                                width: parent.width / 5
                                normalIcon: "image://theme/" + Data.wbIcon(value);
                                checkedIcon: "image://theme/" + Data.wbSelectedIcon(value);
                                value: WhiteBalance.Cloudy
                                savedValue: settings.videoWhiteBalance
                                onClicked: settings.videoWhiteBalance = value;
                                text: Data.wbName(value);
                        }

                        IconButton {
                                width: parent.width / 5
                                normalIcon: "image://theme/" + Data.wbIcon(value);
                                checkedIcon: "image://theme/" + Data.wbSelectedIcon(value);
                                value: WhiteBalance.Flourescent
                                savedValue: settings.videoWhiteBalance
                                onClicked: settings.videoWhiteBalance = value;
                                text: Data.wbName(value);
                        }

                        IconButton {
                                width: parent.width / 5
                                normalIcon: "image://theme/" + Data.wbIcon(value);
                                checkedIcon: "image://theme/" + Data.wbSelectedIcon(value);
                                value: WhiteBalance.Tungsten
                                savedValue: settings.videoWhiteBalance
                                onClicked: settings.videoWhiteBalance = value;
                                text: Data.wbName(value);
                        }
                }
        }

        ToolBar {
                id: toolBar
                anchors.bottom: parent.bottom
                tools: ToolBarLayout {
                        id: layout
                        ToolIcon { iconId: "icon-m-toolbar-back"; onClicked: pageStack.pop(); }
                }
        }
}
