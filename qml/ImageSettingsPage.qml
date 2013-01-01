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
        enableViewfinder: false
        standbyVisible: !Qt.application.active

        Rectangle {
                color: "black"
                anchors.fill: parent
        }

        Flickable {
                id: flickable
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
                                font.pixelSize: 36
                                text: qsTr("Image settings");
                        }

                        SectionHeader {
                                text: qsTr("Aspect ratio");
                        }

                        ButtonRow {
                                anchors.horizontalCenter: parent.horizontalCenter
                                exclusive: false
                                onCheckedButtonChanged: {
                                        // This is needed to initially setup the
                                        // resolutions buttons
                                        imageSettings.resolutions.aspectRatio = checkedButton.aspect;
                                        settings.imageAspectRatio = imageSettings.resolutions.aspectRatio;
                                }

                                Repeater {
                                        model: imageSettings.aspectRatios
                                        delegate: Button {
                                                property string aspect: modelData;
                                                text: qsTr(modelData);
                                                checked: settings.imageAspectRatio == modelData;
                                                onClicked: {
                                                        if (!cam.idle) {
                                                                showError(qsTr("Camera is busy saving."));
                                                                return;
                                                        }

                                                        settings.imageAspectRatio = modelData;
                                                        imageSettings.resolutions.aspectRatio = modelData;
                                                }
                                        }
                                }
                        }

                        SectionHeader {
                                text: qsTr("Resolution");
                        }

                        ButtonRow {
                                id: resolutionsRow
                                anchors.horizontalCenter: parent.horizontalCenter
                                exclusive: false

                                Repeater {
                                        id: resolutions
                                        model: imageSettings.resolutions

                                        // http://stackoverflow.com/questions/1026069/capitalize-the-first-letter-of-string-in-javascript
                                        function name(name, mp) {
                                                return name.charAt(0).toUpperCase() + name.slice(1) + " " + mp + " Mpx";
                                        }

                                        delegate: Button {
                                                property string resolution: resolutionName
                                                property string aspectRatio: resolutionAspectRatio
                                                text: resolutions.name(resolutionName, megaPixels);
                                                checked: settings.imageResolution == resolutionName
                                                onClicked: {
                                                        if (!cam.idle) {
                                                                showError(qsTr("Camera is busy saving."));
                                                                return;
                                                        }

                                                        settings.imageResolution = resolutionName;
                                                }
                                        }
                                }
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
                        ToolIcon { iconId: "icon-m-toolbar-back-white"; onClicked: pageStack.pop(); }
                }
        }
}
