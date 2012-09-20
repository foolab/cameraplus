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

Button {
        id: button
        width: 56
        height: 56

        opacity: 0.5

        property alias widget: __widget.children
        property bool timerConstraints: false
        property alias title: label.text

        checkable: true

        function close() {
                button.checked = false;
        }

        Timer {
                interval: 2000
                running: mouse.enabled && !button.timerConstraints
                repeat: false
                onTriggered: button.close();
        }

        MouseArea {
                id: mouse
                parent: button.parent
                anchors.fill: parent
                enabled: button.checked
                onClicked: button.close();

                Label {
                        id: label
                        y: button.y
                        x: button.x + button.width + 20
                        visible: button.checked
                }

                Item {
                        id: __widget
                        anchors.top: label.bottom
                        anchors.topMargin: 10
//                        y: button.y
                        x: button.x + button.width + 20
                        visible: button.checked
                }
        }
}
