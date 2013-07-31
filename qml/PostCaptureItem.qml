// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
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
import CameraPlus 1.0

Item {
    id: postCaptureItem

    property bool isVideo: itemData.type.search("nmm#Video") > 0
    property alias error: image.error
    property variant itemData: item
    property bool playing: loader.source != ""
    signal clicked

    function startPlayback() {
        loader.source = Qt.resolvedUrl("VideoPlayerPage.qml")
        loader.item.source = itemData.url
        if (!loader.item.play()) {
            showError(qsTr("Error playing video. Please try again."))
            loader.source = ""
        }
    }

    function stopPlayback() {
        if (loader.item) {
            loader.item.stop()
        }
    }

    Loader {
        id: loader
        anchors.fill: parent
    }

    Connections {
        target: loader.item
        onFinished: loader.source = ""
    }

// TODO: rotation
    FullScreenThumbnail {
        id: image
        source: itemData.url
        mimeType: itemData.mimeType

        width: parent.width - 10
        height: parent.height
        anchors.centerIn: parent
        visible: loader.source == ""

        MouseArea {
            id: mouse
            anchors.fill: parent
            enabled: true
            onClicked: postCaptureItem.clicked()
        }

        Column {
            anchors.centerIn: parent
            width: parent.width

            CameraLabel {
                id: errorLabel
                width: parent.width
                visible: image.error
                text: qsTr("Failed to load preview")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 32
            }

            CameraToolIcon {
                id: playIcon
                anchors.horizontalCenter: parent.horizontalCenter
                iconId: "icon-m-toolbar-mediacontrol-play-white"
                visible: isVideo
                onClicked: startPlayback()
            }
        }
    }
}
