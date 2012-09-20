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

Selector {
        id: button

        property alias value: flash.value

        iconSource: flashIcon(flash.value);

        title: qsTr("Flash mode");

        Flash {
                // TODO: move this out of here.
                id: flash
                camera: cam
                value: settings.imageFlashMode
		// TODO: scene modes can change flash value. what to do here ?
                onValueChanged: settings.imageFlashMode = value;
        }

        function flashIcon(val) {
                var x = row.children.length;
                var i = 0;
                for (i = 0; i < x; i++) {
                        if (row.children[i].value == val) {
                                return row.children[i].normalIcon;
                        }
                }
        }

        widget: Row {
                id: row
                height: button.checked ? 64 : 0
                width: button.checked ? (children.length * height) +  (children.length - 1) * spacing : 0
                spacing: 10

                Behavior on width {
                        // TODO: seems animation is not working
                        PropertyAnimation { duration: 250; }
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-flash-auto"
                        checkedIcon: "image://theme/icon-m-camera-flash-auto-pressed"
                        onClicked: settings.imageFlashMode = value;
                        value: Flash.Auto
                        savedValue: settings.imageFlashMode
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-flash-always"
                        checkedIcon: "image://theme/icon-m-camera-flash-always-pressed"
                        onClicked: settings.imageFlashMode = value;
                        value: Flash.On
                        savedValue: settings.imageFlashMode
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-flash-off"
                        checkedIcon: "image://theme/icon-m-camera-flash-off-pressed"
                        onClicked: settings.imageFlashMode = value;
                        value: Flash.Off
                        savedValue: settings.imageFlashMode
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-flash-red-eye"
                        checkedIcon: "image://theme/icon-m-camera-flash-red-eye-pressed"
                        onClicked: settings.imageFlashMode = value;
                        value: Flash.RedEye
                        savedValue: settings.imageFlashMode
                }
        }
}
