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

Item {
        id: button

        property url iconSource
        property Style platformStyle: ButtonStyle {}

        property alias pressed: mouse.pressed
        property alias containsMouse: mouse.containsMouse
        property alias mouseX: mouse.mouseX
        property alias mouseY: mouse.mouseY

        signal clicked
        signal exited

        MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: button.clicked();
                onExited: button.exited();
        }

        BorderImage {
                id: background
                anchors.fill: parent
                border.left: button.platformStyle.backgroundMarginLeft
                border.top: button.platformStyle.backgroundMarginTop
                border.right: button.platformStyle.backgroundMarginRight
                border.bottom: button.platformStyle.backgroundMarginBottom
                source:  pressed ? button.platformStyle.pressedBackground : button.platformStyle.background
        }

        Image {
                id: icon
                anchors.centerIn: parent
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -1
                source: button.iconSource
                visible: source != ""
        }
}