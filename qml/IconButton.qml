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
        id: root
        width: Math.max(150, Math.max(label.width, icon.width) + 20);
        height: 120

        property string normalIcon: ""
        property string checkedIcon: ""
        property int value: -1
        property int savedValue: -1

        property alias iconSource: icon.source
        property alias text: label.text

        signal clicked

        MouseArea {
                anchors.fill: parent
                onClicked: root.clicked();
        }

        Image {
                id: icon

                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.topMargin: 10
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                source: root.value == root.savedValue ? checkedIcon : normalIcon
                fillMode: Image.PreserveAspectFit
        }

        Label {
                id: label
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.top: icon.bottom
                anchors.bottomMargin: 10
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                anchors.topMargin: 10
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.NoWrap
                font.pixelSize: 22
        }
}
