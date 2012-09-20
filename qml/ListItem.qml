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

// This mimics MListItem

Rectangle {
        signal clicked

        property alias title: title.text
        property alias subtitle: subtitle.text

        property string iconId: ""

        height: 100

        id: button
        color: mouse.pressed ? "#464646" : "black"

        MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: button.clicked();
        }

        Image {
                id: icon
                anchors.top: button.top
                anchors.bottom: button.bottom
                anchors.left: button.left
                width: height
                height: button.height
                anchors.margins: 10
                source: button.iconId != "" ? "image://theme/" + button.iconId : ""
        }

        Label {
                id: title
                anchors.top: button.top
                anchors.left: icon.right
                anchors.right: button.right
                anchors.margins: 10
                font.bold: true
        }

        Label {
                id: subtitle
                anchors.top: title.bottom
                anchors.bottom: button.bottom
                anchors.left: icon.right
                anchors.right: button.right
                anchors.margins: 10
                platformStyle: LabelStyle {
                        textColor: "lightgray"
                }
        }
}
