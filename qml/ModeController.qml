// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2014 Mohammed Sameer <msameer@foolab.org>
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

import QtQuick 2.0
import QtCamera 1.0

Rectangle {
    id: controller
    property Camera cam
    property alias busy: animation.running

    z: 1
    anchors.fill: parent
    color: "black"
    opacity: 0

    SequentialAnimation {
        id: animation

        alwaysRunToEnd: true
        running: cam.running && (activePlugin.uuid != settings.plugin || cam.deviceId != settings.device)

        PropertyAnimation {
            properties: "opacity"
            target: controller
            duration: 150
            to: 1.0
        }

        PauseAnimation { duration: 50 }
        ScriptAction {
                script: {
                        var restart = false

                        if (cam.deviceId != settings.device) {
                                viewfinder.cameraDeviceChanged()
                                // Reset pipeline manager error
                                pipelineManager.error = false
                                restart = true
                        }

                        root.resetCamera(settings.device)

                        if (restart) {
                                pipelineManager.startCamera()
                        }
                }
        }

        PropertyAnimation {
            properties: "opacity"
            target: controller
            duration: 50
            to: 0
        }
    }
}
