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
import CameraPlus 1.0

Flickable {
    id: flick

    property bool playing: loader.source != ""
    property string fileName
    property url url

    boundsBehavior: Flickable.StopAtBounds
    width: parent.width
    height: parent.height
    contentWidth: width
    contentHeight: height

    Component.onCompleted: {
        fileName = media.fileName
        url = media.url
        load()
    }

    function load() {
        image.load(media)
    }

    function deleteUrl() {
        deleteAnimation.start()
    }

    ImageThumbnail {
        id: image
        displayLevel: QuillItem.DisplayLevelFullScreen

        property bool busy: deleteAnimation.running

        anchors.centerIn: parent
        width: Math.max(flick.width, flick.contentWidth)
        height: Math.max(flick.height, flick.contentHeight)

        function resetZoom() {
            flick.resizeContent(postCaptureView.width, postCaptureView.height,
                Qt.point(postCaptureView.width / 2, postCaptureView.height / 2))
            flick.contentX = 0
            flick.contentY = 0
        }

        function load(media) {
            resetZoom()
            initialize(media.url, media.mimeType, image.displayLevel)
        }

        function unload() {
            clear()
        }

        SequentialAnimation {
            id: deleteAnimation

            PropertyAnimation {
                target: flick
                properties: "x"
                from: 0
                to: width
                duration: 250
            }

            ScriptAction {
                script: {
                    if (!remove.remove(media.url)) {
                        showError(qsTr("Failed to delete item"))
                    } else {
                        postCaptureModel.remove(media.url)
                    }

                    flick.x = 0
                }
            }
        }

    }

    PinchArea {
        id: pinchArea
        width: Math.max(flick.width, flick.contentWidth)
        height: Math.max(flick.height, flick.contentHeight)
        enabled: !playIcon.visible
        property real initialWidth: image.width
        property real initialHeight: image.height

        pinch.minimumScale: 1
        pinch.maximumScale: 4
        onPinchFinished: flick.returnToBounds()
        onPinchStarted: {
            initialWidth = image.width * image.scale
            initialHeight = image.height * image.scale
        }

        onPinchUpdated: {
            var scale = pinch.scale;
            var newWidth = Math.max(initialWidth * scale, postCaptureView.width)
            var newHeight = Math.max(initialHeight * scale, postCaptureView.height)

            newWidth = Math.max(newWidth, postCaptureView.width * pinchArea.pinch.minimumScale)
            newWidth = Math.min(newWidth, postCaptureView.width * pinchArea.pinch.maximumScale)

            newHeight = Math.max(newHeight, postCaptureView.height * pinchArea.pinch.minimumScale)
            newHeight = Math.min(newHeight, postCaptureView.height * pinchArea.pinch.maximumScale)

            var center = pinch.center
            if (newWidth == postCaptureView.width) {
                center.x = postCaptureView.width / 2
            }

            if (newHeight == postCaptureView.height) {
                center.y = postCaptureView.height / 2
            }

            flick.resizeContent(newWidth, newHeight, center)
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: toggleImageList = !toggleImageList
        onDoubleClicked: image.resetZoom()
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
            visible: media.video
            onClicked: loader.startPlayback(media.url)
        }
    }
}
