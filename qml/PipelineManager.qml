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

@IMPORT_QT_QUICK@
import QtCamera 1.0
import CameraPlus 1.0

Item {
    id: handler

    property bool showStandBy: state != "on"

    property alias acquired: policy.acquired
    property alias hijacked: policy.hijacked
    property alias scaleAcquired: policy.scaleAcquired

    property Camera camera: null
    property Item currentItem
    property bool error: false
    property int currentPolicyMode: CameraResources.None

    onCurrentItemChanged: {
        if (state == "on" || state == "policyLost") {
            startCamera()
        }
    }

    Connections {
        target: currentItem
        onPolicyModeChanged: {
            if (state == "on" || state == "policyLost") {
                startCamera()
            }
        }
    }

    CameraResources {
        id: policy
    }

    function startCamera() {
        if (error) {
            return
        } else if ((currentPolicyMode == currentItem.policyMode) && cam.running) {
            return
        } else if (!policy.acquire(currentItem.policyMode)) {
            console.log("Failed to acquire policy resources")
            return
        } else if (currentItem.policyMode == CameraResources.Player) {
            currentPolicyMode = CameraResources.Player
            camera.stop(true)
        } else if (!camera.start()) {
            showError(qsTr("Failed to start camera. Please restart the application."))
        } else {
            currentPolicyMode = currentItem.policyMode
        }
    }

    function stopCamera() {
        if (camera.stop(false)) {
            policy.acquire(CameraResources.None)
            currentPolicyMode = CameraResources.None
            error = false
        }
    }

    function forceStopCamera() {
        // We don't release resources here so we can get them back
        // when they become available
        currentItem.policyLost()
        camera.stop(true)
        currentPolicyMode = CameraResources.None
        error = false
    }

    state: "off"

//    onStateChanged: console.log("New state " + handler.state);

    states: [
        State {
            name: "on"
            when: Qt.application.active && currentItem && currentItem.policyMode != CameraResources.None && !policy.hijacked
        },
        State {
            name: "off"
            when: (!Qt.application.active && camera.idle) || (currentItem && currentItem.policyMode == CameraResources.None && camera.idle)
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
            to: "off"
            ScriptAction {
                script: stopCamera()
            }
        },
        Transition {
            from: "off"
            to: "on"
            ScriptAction {
                script: handler.startCamera()
            }
        },
        Transition {
            from: "on"
            to: "policyLost"
            ScriptAction {
                script: forceStopCamera()
            }
        },
        Transition {
            from: "policyLost"
            to: "off"
            ScriptAction {
                script: stopCamera()
            }
        },
        Transition {
            from: "policyLost"
            to: "on"
            ScriptAction {
                script: startCamera()
            }
        }
    ]
}
