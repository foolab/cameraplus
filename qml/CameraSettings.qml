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

Column {
        id: col
        spacing: 10

        Label {
                font.pixelSize: 36
                text: qsTr("Camera settings");
        }

        SectionHeader {
                text: qsTr("Show captured content");
        }

        ButtonRow {
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                        text: qsTr("Disabled");
                        checked: settings.postCaptureTimeout == 0;
                        onClicked: settings.postCaptureTimeout = 0;
                }

                Button {
                        text: qsTr("2 seconds");
                        checked: settings.postCaptureTimeout == 2;
                        onClicked: settings.postCaptureTimeout = 2;
                }

                Button {
                        text: qsTr("5 seconds");
                        checked: settings.postCaptureTimeout == 10;
                        onClicked: settings.postCaptureTimeout = 10;
                }

                Button {
                        text: qsTr("No timeout");
                        checked: settings.postCaptureTimeout == -1;
                        onClicked: settings.postCaptureTimeout = -1;
                }
        }

        SectionHeader {
                text: qsTr("Creator name");
        }

        TextField {
                placeholderText: qsTr("Name or copyright");
                width: parent.width
                text: settings.creatorName
                onTextChanged: settings.creatorName = text;
        }

        Item {
                width: parent.width
                height: Math.max(enableCameraSoundsLabel.height, enableCameraSounds.height);

                Label {
                        id: enableCameraSoundsLabel
                        anchors.left: parent.left
                        text: qsTr("Enable camera sounds");
                }

                Switch {
                        id: enableCameraSounds
                        anchors.right: parent.right
                        // We have to do it that way because QML complains about a binding
                        // loop for checked if we bind the checked property to the settings value.
                        Component.onCompleted: checked = settings.soundEnabled;
                        onCheckedChanged: settings.soundEnabled = checked;
                }
        }

        Item {
                width: parent.width
                height: Math.max(useGpsLabel.height, useGps.height);

                Label {
                        id: useGpsLabel
                        anchors.left: parent.left
                        text: qsTr("Use GPS");
                }

                Switch {
                        id: useGps
                        anchors.right: parent.right
                        // We have to do it that way because QML complains about a binding
                        // loop for checked if we bind the checked property to the settings value.
                        Component.onCompleted: checked = settings.useGps;
                        onCheckedChanged: settings.useGps = checked;
                }
        }

        Item {
                width: parent.width
                height: Math.max(useGeotagsLabel.height, useGeotags.height);

                // TODO: transition when hiding/showing and we should scroll a bit to show it
                visible: useGps.checked

                Label {
                        id: useGeotagsLabel
                        anchors.left: parent.left
                        text: qsTr("Use geotags");
                }

                Switch {
                        id: useGeotags
                        anchors.right: parent.right
                        // We have to do it that way because QML complains about a binding
                        // loop for checked if we bind the checked property to the settings value.
                        Component.onCompleted: checked = settings.useGeotags;
                        onCheckedChanged: settings.useGeotags = checked;
                }
        }
}
