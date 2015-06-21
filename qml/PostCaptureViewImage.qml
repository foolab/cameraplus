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
    property string mime

    signal playClicked

    boundsBehavior: Flickable.StopAtBounds
    width: parent.width
    height: parent.height
    contentWidth: width
    contentHeight: height

    Component.onCompleted: {
        fileName = media.fileName
        url = media.url
        mime = media.mimeType
    }

    function load() {
        image.load(media)
    }

    function deleteUrl() {
        deleteAnimation.start()
    }

    function deleteUrlNow() {
        deleteAnimation.stop()

        if (!deleteHelper.remove(media.url)) {
            return false
        }

        postCaptureModel.remove(media.url)

        return true
    }

    QuillItem {
        id: image
        property bool busy: deleteAnimation.running

        anchors.centerIn: parent
        width: parent.width
        height: parent.height

        state: "landscape"
        states: [
            State {
                name: "portrait"
                when: isPortrait
            },
            State {
                name: "landscape"
                when: !isPortrait
            }
        ]

        transitions: [
            Transition {
                to: "landscape"
                SequentialAnimation {
                    ParallelAnimation {
                        PropertyAnimation { property: "width"; duration: 100; to: image.width / 2; target: image}
                        PropertyAnimation { property: "height"; duration: 100; to: image.height / 2; target: image}
                    }
                    PropertyAnimation { property: "rotation"; duration: 200; to: 0; target: image}
                    ParallelAnimation {
                        PropertyAnimation { property: "width"; duration: 100; to: image.parent.width; target: image}
                        PropertyAnimation { property: "height"; duration: 100; to: image.parent.height; target: image}
                    }
                }
            },
            Transition {
                to: "portrait"
                SequentialAnimation {
                    ParallelAnimation {
                        PropertyAnimation { property: "width"; duration: 100; to: image.width / 2; target: image}
                        PropertyAnimation { property: "height"; duration: 100; to: image.height / 2; target: image}
                    }
                    PropertyAnimation { property: "rotation"; duration: 200; to: -90; target: image}
                    ParallelAnimation {
                        PropertyAnimation { property: "width"; duration: 100; to: image.parent.height; target: image}
                        PropertyAnimation { property: "height"; duration: 100; to: image.parent.width; target: image}
                    }
                }
            }
        ]

        url: media.url
        mimeType: media.mimeType
        displayLevel: QuillItem.DisplayLevelFullScreen
        priority: QuillItem.PriorityHigh

        function resetZoom() {
            flick.resizeContent(postCaptureView.width, postCaptureView.height,
                Qt.point(postCaptureView.width / 2, postCaptureView.height / 2))
            flick.contentX = 0
            flick.contentY = 0
        }

        function load(media) {
            resetZoom()
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
                    if (!deleteUrlNow()) {
                        showError(qsTr("Failed to delete item"))
                    }

                    flick.x = 0
                }
            }
        }
    }

    PinchArea {
        id: pinchArea
        anchors.fill: parent
        enabled: !playIcon.visible
        property real initialWidth: image.width
        property real initialHeight: image.height

        pinch.minimumScale: 1
        pinch.maximumScale: 4
        onPinchFinished: flick.returnToBounds()

        function clamp(num, min, max) {
            return num < min ? min : (num > max ? max : num)
        }

        onPinchStarted: {
            initialWidth = image.width * image.scale
            initialHeight = image.height * image.scale
            image.displayLevel = QuillItem.DisplayLevelLarge
        }

        onPinchUpdated: {
            var scale = pinch.scale;
            var newWidth = Math.max(initialWidth * scale, postCaptureView.width)
            var newHeight = Math.max(initialHeight * scale, postCaptureView.height)

            newWidth = clamp(newWidth, postCaptureView.width * pinchArea.pinch.minimumScale,
                postCaptureView.width * pinchArea.pinch.maximumScale)

            newHeight = clamp(newHeight, postCaptureView.height * pinchArea.pinch.minimumScale,
                postCaptureView.height * pinchArea.pinch.maximumScale)

            var center = pinch.center
            if (newWidth == postCaptureView.width) {
                center.x = postCaptureView.width / 2
            }

            if (newHeight == postCaptureView.height) {
                center.y = postCaptureView.height / 2
            }

            flick.resizeContent(newWidth, newHeight, center)
        }

        MouseArea {
            anchors.fill: parent
            onClicked: toggleImageList = !toggleImageList
            onDoubleClicked: image.resetZoom()
        }
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
            font.pixelSize: cameraStyle.fontSizeMedium
        }

        CameraToolIcon {
            id: playIcon
            anchors.horizontalCenter: parent.horizontalCenter
            iconSource: cameraTheme.videoPlayIconId
            visible: media.video
            onClicked: flick.playClicked()
        }
    }
}
