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

    property url itemUrl: url
    property string itemTitle: title
    property string itemMimeType: mimeType
    property string itemCreated: created
    property string itemFileName: fileName

    property bool isVideo: itemMimeType.search("video/") >= 0
    property alias error: image.error
    property bool playing: loader.source != ""
    signal clicked
    clip: true

    function startPlayback() {
        loader.source = Qt.resolvedUrl("VideoPlayerPage.qml")
        loader.item.source = itemUrl
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

    ImageThumbnail {
        id: image
        source: itemUrl
        mimeType: itemMimeType
        rotation: calculateRotation(orientation.orientation)
        width: isPortrait ? parent.height : parent.width - 10
        height: isPortrait ? parent.width - 10 : parent.height
        anchors.centerIn: parent
        visible: loader.source == ""
        property bool isPortrait: rotation == -90

        Behavior on width {
            PropertyAnimation { duration: 100 }
        }

        Behavior on height {
            PropertyAnimation { duration: 100 }
        }

        Behavior on rotation {
            PropertyAnimation { duration: 100 }
        }

        function calculateRotation(orientation) {
            switch (orientation) {
                case CameraOrientation.InvertedLandscape:
                case CameraOrientation.Landscape:
                    return 0
                case CameraOrientation.InvertedPortrait:
                case CameraOrientation.Portrait:
                    return -90
                default:
                    return 0
            }
        }

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
                iconSource: cameraTheme.videoPlayIconId
                visible: isVideo
                onClicked: startPlayback()
            }
        }
    }
}
