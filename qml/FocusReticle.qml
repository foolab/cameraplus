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

Item {
        anchors.fill: parent
        property int cafStatus: AutoFocus.None
        property int status: AutoFocus.None

        onStatusChanged: {
                if (status != AutoFocus.Running) {
                        reticle.visible = true;
                }
        }

        function predictColor(caf, status) {
                if (status == AutoFocus.Success) {
                        return "steelblue";
                }
                else if (status == AutoFocus.Fail) {
                        return "red";
                }
                else if (status == AutoFocus.Running) {
                        return "white";
                }
                else if (caf == AutoFocus.Success) {
                        return "steelblue";
                }
                else {
                        return "white";
                }
        }

        FocusRectangle {
                id: reticle
                width: 250
                height: 150
                anchors.centerIn: parent
                color: predictColor(cafStatus, status);
        }

        Timer {
                interval: 500
                running: status == AutoFocus.Running
                triggeredOnStart: true
                repeat: true
                onTriggered: reticle.visible = !reticle.visible
        }
}
