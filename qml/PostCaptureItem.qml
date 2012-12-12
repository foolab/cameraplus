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
import CameraPlus 1.0

Item {
        id: item
        property bool isImage: mimetype.toLowerCase().search("video") < 0
        property bool error: false

        property bool isCurrentItem: PathView.isCurrentItem
        onIsCurrentItemChanged: page.currentItem = item;

        property string fileName: filename
        property string creationDate: created

        function startPlayback() {
                openFileNow("VideoPlayerPage.qml");
                pageStack.currentPage.source = url;
                pageStack.currentPage.play();
        }

        Label {
                anchors.fill: parent
                visible: image.error && page.status == PageStatus.Active
                text: qsTr("Failed to load preview");
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 32
        }

        QuillItem {
                id: image
                anchors.fill: parent
                visible: page.status == PageStatus.Activating || page.status == PageStatus.Active

                Component.onCompleted: initialize(url, mimetype);

                MouseArea {
                        id: mouse
                        anchors.fill: parent
                        enabled: true
                        onClicked: toolBar.visible = !toolBar.visible
                }

                ToolIcon {
                        id: playIcon
                        anchors.centerIn: parent
                        iconSource: "image://theme/icon-s-music-video-play"
                        visible: !isImage
                        onClicked: startPlayback();
                }
        }
}
