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
        policyMode: CameraResources.Image
        needsPipeline: false

        Rectangle {
                color: "black"
                anchors.fill: parent
        }

        Flickable {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: toolBar.top
                anchors.margins: 10
                contentHeight: col.height

                Column {
                        id: col

                        width: parent.width
                        spacing: 10

                        Label {
                                // TODO:
                                text: qsTr("Image settings");
                        }

                        SectionHeader {
                                text: qsTr("Capture mode");
                        }

                        ButtonRow {
                                anchors.horizontalCenter: parent.horizontalCenter
                                // TODO:
                                Button { text: qsTr("Normal"); }
                                Button { text: qsTr("Self timer"); }
                                Button { text: qsTr("Fast capture"); }
                        }

                        Row {
                                width: parent.width

                                ListItem {
                                        id: wb
                                        width: parent.width / 2
                                        title: qsTr("White balance");
                                        subtitle: Data.wbName(settings.imageWhiteBalance);
                                        iconId: Data.wbSelectedIcon(settings.imageWhiteBalance);
                                        onClicked: openFile("ImageWhiteBalancePage.qml");
                                }

                                ListItem {
                                        id: cf
                                        width: parent.width / 2
                                        title: qsTr("Color filter");
                                        subtitle: Data.cfName(settings.imageColorFilter);
                                        iconId: Data.cfSelectedIcon(settings.imageColorFilter);
                                        onClicked: openFile("ImageColorFilterPage.qml");
                                }
                        }

                        SectionHeader {
                                text: qsTr("Self timer");
                        }

                        ButtonRow {
                                anchors.horizontalCenter: parent.horizontalCenter
                                // TODO:
                                Button { text: qsTr("2 seconds"); }
                                Button { text: qsTr("10 seconds"); }
                        }

                        SectionHeader {
                                text: qsTr("Light sensitivity");
                        }

                        ButtonRow {
                                anchors.horizontalCenter: parent.horizontalCenter

                                Button {
                                        text: qsTr("Automatic");
                                        checked: settings.imageIso == 0;
                                        onClicked: settings.imageIso = 0;
                                }

                                Button {
                                        text: qsTr("ISO 100");
                                        checked: settings.imageIso == 100;
                                        onClicked: settings.imageIso = 100;
                                }

                                Button {
                                        text: qsTr("ISO 200");
                                        checked: settings.imageIso == 200;
                                        onClicked: settings.imageIso = 200;
                                }

                                Button {
                                        text: qsTr("ISO 400");
                                        checked: settings.imageIso == 400;
                                        onClicked: settings.imageIso = 400;
                                }

                                Button {
                                        text: qsTr("ISO 800");
                                        checked: settings.imageIso == 800;
                                        onClicked: settings.imageIso = 800;
                                }
                        }

                        SectionHeader {
                                text: qsTr("Aspect ratio");
                        }

                        ButtonRow {
                                anchors.horizontalCenter: parent.horizontalCenter
                                // TODO:
                                Button { text: qsTr("16:9"); }
                                Button { text: qsTr("4:3"); }
                                Button { text: qsTr("3:2"); }
                        }

                        SectionHeader {
                                text: qsTr("Resolution");
                        }

                        ButtonRow {
                                anchors.horizontalCenter: parent.horizontalCenter
                                // TODO:
                                Button {}
                                Button {}
                                Button {}
                        }

                        CameraSettings {
                                anchors.horizontalCenter: parent.horizontalCenter
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
