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
import Sailfish.Silica 1.0

MouseArea {
    property alias checked: switchItem.checked
    property alias text: label.text
    property bool down: pressed && containsMouse

    width: switchItem.width + label.width
    height: Math.max(switchItem.height, label.height)

    Switch {
        id: switchItem
        width: Theme.itemSizeExtraSmall
        height: Theme.itemSizeSmall
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        automaticCheck: false

        // We need to propagate the mouse click event to the "parent" MouseArea
        propagateComposedEvents: true
        onClicked: mouse.accepted = false
    }

    CameraLabel {
        id: label
        height: Theme.itemSizeSmall
        anchors.left: switchItem.right
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        color: down ? Theme.highlightColor : Theme.primaryColor
        font.capitalization: Font.Capitalize
    }
}
