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
import QtCamera 1.0

Item {
    id: postCaptureView

    property Camera camera: viewfinder.camera
    property bool pressed: image.playing
    property int policyMode: image.playing ? CameraResources.Player : settings.mode == Camera.VideoMode ? CameraResources.Video : CameraResources.Image
    property bool inhibitDim: image.playing

    property bool toggleImageList: true
    property bool hideImageList: image.playing || toggleImageList

    Component.onCompleted: postCaptureModel.reload()

    ImageThumbnail {
        id: image
        property variant currentMedia
        property bool isVideo: currentMedia ? currentMedia.mimeType.search("video/") >= 0 : false
        property bool playing: loader.source != ""
        property bool busy: deleteAnimation.running

        width: parent.width
        height: parent.height

        function deleteUrl() {
            deleteAnimation.start()
        }

        SequentialAnimation {
            id: deleteAnimation

            PropertyAnimation {
                target: image
                properties: "x"
                from: 0
                to: width
                duration: 250
            }

            ScriptAction {
                script: {
                    if (!remove.remove(image.currentMedia.url)) {
                        showError(qsTr("Failed to delete item"))
                    } else {
                        postCaptureModel.remove(image.currentMedia.url)
                    }

                    image.x = 0
                }
            }
        }

        function load(media) {
            initialize(media.url, media.mimeType, 0)
            currentMedia = media
        }

        MouseArea {
            anchors.fill: parent
            onClicked: toggleImageList = !toggleImageList
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
                visible: image.isVideo
                onClicked: loader.startPlayback(image.currentMedia.url)
            }
        }
    }

    ListView {
        id: thumbnails
        anchors.bottom: parent.bottom
        anchors.bottomMargin: hideImageList ? -120 : 0
        anchors.right: parent.right
        anchors.left: parent.left
        height: 120
        onCurrentItemChanged: currentItem.load()

        orientation: ListView.Horizontal
        model: postCaptureModel
        visible: anchors.bottomMargin > -100
        enabled: !image.busy

        Behavior on anchors.bottomMargin {
            NumberAnimation { duration: 200 }
        }

        delegate: Rectangle {
            id: rectangle
            property bool isVideo: media.mimeType.search("video/") >= 0
            width: 120
            height: 120
            color: isVideo ? "blue" : "white"

            scale: mouse.pressed ? 2 : 1
            z: scale > 1 ? 1 : 0

            transformOrigin: Item.Bottom

            Behavior on scale {
                NumberAnimation { duration: 200 }
            }

            function load() {
                image.load(media);
            }

            MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: {
                    if (thumbnails.currentItem == rectangle) {
                        rectangle.load()
                    } else {
                        thumbnails.currentIndex = index
                    }
                }
            }

            ImageThumbnail {
                id: thumbnail
                width: 116
                height: 116
                anchors.centerIn: parent
                source: media.url
                mimeType: media.mimeType
                displayLevel: 1
            }
        }
    }

    Loader {
        id: loader
        anchors.fill: parent

        function startPlayback(url) {
            loader.source = Qt.resolvedUrl("VideoPlayerPage.qml")
            loader.item.source = url
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

        Connections {
            target: loader.item
            onFinished: loader.source = ""
        }
    }

    CameraToolBar {
        id: toolBar
        anchors.top: parent.top
        anchors.topMargin: hideImageList ? -100 : 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        opacity: 0.5
        targetWidth: parent.width - (anchors.leftMargin * 2)
        expanded: true
        hideBack: true
        enabled: !image.busy

        Behavior on anchors.topMargin {
            NumberAnimation { duration: 200 }
        }

        tools: CameraToolBarTools {
            CameraToolIcon {
                iconSource: cameraTheme.shareIconId
                onClicked: share.shareUrl(image.currentMedia.url)
            }

            CameraToolIcon {
                iconSource: cameraTheme.deleteIconId
                onClicked: deleteDialog.deleteUrl(image.currentMedia.url, image.currentMedia.fileName)
            }

            CameraToolIcon {
                iconSource: cameraTheme.galleryIconId
                onClicked: gallery.launchGallery()
            }

            CameraLabel {
                height: toolBar.height
                text: image.currentMedia.fileName
                width: 350
                font.pixelSize: 32
                font.bold: true
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    ShareHelper {
        id: share
        settings: platformSettings

        function shareUrl(url) {
            if (url != "" && !share.share(url)) {
                showError(qsTr("Failed to launch share service"))
            }
        }
    }

    GalleryHelper {
        id: gallery
        settings: platformSettings

        function launchGallery() {
            if (!gallery.launch()) {
                showError(qsTr("Failed to launch gallery"))
            }
        }
    }

    CameraQueryDialog {
        id: deleteDialog
        titleText: qsTr("Delete item?");
        acceptButtonText: qsTr("Yes");
        rejectButtonText: qsTr("No");

        onAccepted: image.deleteUrl()

        DeleteHelper {
            id: remove
        }

        function deleteUrl(url, fileName) {
            if (url == "" || fileName == "") {
                return
            }

            deleteDialog.messageText = fileName
            deleteDialog.open()
        }
    }

    PostCaptureModel {
        id: postCaptureModel
        imagePath: platformSettings.imagePath
        videoPath: platformSettings.videoPath
    }

    function policyLost() {
        loader.stopPlayback()
    }
}
