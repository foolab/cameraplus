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
import QtCamera 1.0
import CameraPlus 1.0

// TODO: cleanup debug output
// TODO: losing resources will freeze camera somehow.

Item {
        id: handler

        property bool showStandBy: state != "on"

        property Camera camera: null
        property Item currentPage: pageStack.currentPage

        onCurrentPageChanged: {
                if (state == "on") {
                        startCamera();
                }
        }

        CameraResources {
                id: policy
        }

        function startCamera() {
                console.log("start");
                if (!policy.acquire(currentPage.policyMode)) {
                        console.log("Failed to acquire policy resources");
                        camera.stop(true);
                        return;
                }

                if (!camera.start()) {
                        showError(qsTr("Failed to start camera. Please restart the application."));
                }
        }

        function stopCamera() {
                console.log("stop");
                camera.stop(false);
                policy.acquire(CameraResources.None);
        }

        function forceStopCamera() {
                // We don't release resources here so we can get them back when they become available
                console.log("force");
                camera.stop(true);
                showError(qsTr("Resources lost. Stopping camera."));
        }

        state: "off"

        onStateChanged: console.log("New state " + handler.state);

        states: [
        State {
                name: "on"
                when: Qt.application.active && currentPage && currentPage.needsPipeline && !policy.hijacked
        },
        State {
                name: "off"
                when: (!Qt.application.active && camera.idle) || (currentPage && !currentPage.needsPipeline)
        },
        State {
                name: "policyLost"
                when: policy.hijacked
        },
        State {
                name: "error"
        }
        ]

        transitions: [
        Transition {
                from: "on"
                to: "off"
                ScriptAction {
                        script: stopCamera();
                }
        },
        Transition {
                from: "off"
                to: "on"
                ScriptAction {
                        script: handler.startCamera();
                }
        },

        Transition {
                from: "on"
                to: "policyLost"
                ScriptAction {
                        script: forceStopCamera();
                }
        },

        Transition {
                from: "policyLost"
                to: "off"
                ScriptAction {
                        script: stopCamera();
                }
        },
        Transition {
                from: "policyLost"
                to: "on"
                ScriptAction {
                        script: startCamera();
                }
        }
        ]
}
