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
import QtCamera 1.0

Dialog {
        id: dialog

        content: item

        Connections {
                target: Qt.application
                onActiveChanged: {
                        if (!Qt.application.active) {
                                dialog.close();
                        }
                }
        }

        onStatusChanged: {
                if (status == DialogStatus.Open) {
                        cam.renderingEnabled = false;
                }
                else if (status == DialogStatus.Closing) {
                        cam.renderingEnabled = true;
                }
        }

        Item {
                id: item
                width: parent.width
                height: root.height


                Flickable {
                        id: flickable
                        anchors.fill: parent
                        anchors.margins: 10
                        contentHeight: col.height

                        Column {
                                id: col

                                width: parent.width
                                spacing: 10

                                Label {
                                        font.pixelSize: 36
                                        text: qsTr("Video settings");
                                }

                                VideoResolutionSettings {
                                        width: parent.width
                                }

                                CameraSettings {
                                        anchors.horizontalCenter: parent.horizontalCenter
                                }
                        }
                }
        }
}
