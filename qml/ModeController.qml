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

Item {
        id: controller
        property int mode: settings.mode
        property Camera cam: null
        property Item dimmer: null
        property alias animationRunning: animation.running

        SequentialAnimation {
                id: animation
                property int mode: Camera.UnknownMode

                function setMode() {
                        root.resetCamera(cam.deviceId, mode);
                }

                NumberAnimation { target: dimmer; property: "opacity"; from: 0; to: 1; duration: 150; alwaysRunToEnd: true }

                PauseAnimation { duration: 50 }

                ParallelAnimation {
                        ScriptAction { script: animation.setMode(); }
                        PauseAnimation { duration: 200 }
                }

                PauseAnimation { duration: 50 }

                NumberAnimation { target: dimmer; property: "opacity"; from: 1; to: 0; duration: 150; alwaysRunToEnd: true }
        }

        onModeChanged: {
                set(mode);
        }

        function set(newMode) {
                if (!cam) {
                        return;
                }

                if (cam.mode == newMode) {
                        return;
                }

                animation.mode = newMode;

                animation.start();
        }
}
