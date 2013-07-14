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
import CameraPlus 1.0

CameraPage {
        id: page

        controlsVisible: false
        policyMode: CameraResources.None
        activationData: ControlsActivationData {standbyVisible: !Qt.application.active}

        property Item currentItem: null
        property bool available: currentItem ? currentItem.itemData.available : false

        Component.onCompleted: postCaptureModel.reload();

        Rectangle {
                color: "black"
                anchors.fill: parent
        }

        PathView {
                id: view
                anchors.fill: parent

                path: Path {
                        startX: - view.width
                        startY: view.height / 2
                        PathLine { x: view.width * 2; y: view.height / 2 }
                }

                // Insanely high value to prevent panning multiple images
                flickDeceleration: 999999

                preferredHighlightBegin: 0.5
                preferredHighlightEnd: 0.5
                highlightRangeMode: PathView.StrictlyEnforceRange
                pathItemCount: 3

                model: PostCaptureModel {
                        id: postCaptureModel
                        manufacturer: deviceInfo.manufacturer
                        model: deviceInfo.model
                        onError: {
                                console.log("Error populating model " + msg);
                                showError(qsTr("Failed to load captures"));
                        }
                }

                Label {
                        // TODO: Hide this when we have no items
                        text: qsTr("No captures available");
                        anchors.centerIn: parent
                        font.pixelSize: 36
                        visible: currentItem == null
                }

                delegate: PostCaptureItem {
                        width: view.width
                        height: view.height
                        onClicked: hideTimer.running = !hideTimer.running;
                }
        }
}
