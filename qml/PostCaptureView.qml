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
import QtCamera 1.0

Item {
    id: postCaptureView

    property Camera camera: viewfinder.camera
    property bool pressed: view.currentItem ? view.currentItem.playing : false
    property int policyMode: pressed ? CameraResources.Player : activePlugin.mode == Camera.VideoMode ? CameraResources.Video : CameraResources.Image
    property bool inhibitDim: pressed

    property bool toggleImageList: true
    property bool hideImageList: pressed || toggleImageList
    property bool isPortrait: cameraPosition.isPortrait

    Component.onCompleted: postCaptureModel.reload()

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    ListView {
        id: view
        anchors.fill: parent
        orientation: ListView.Vertical
        model: postCaptureModel
        snapMode: ListView.SnapOneItem
        onCurrentIndexChanged: thumbnails.positionViewAtIndex(currentIndex, ListView.Contain)
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 1
        highlightResizeDuration: 1
        highlightRangeMode: ListView.StrictlyEnforceRange
        interactive: currentItem ? !currentItem.busy : true
        // We cache some images. This should speed up loading a bit and prevent
        // rotation when we flick images in portrait mode
        cacheBuffer: height * 2

        delegate: PostCaptureViewImage {
            id: image
            width: view.width
            height: view.height
            onPlayClicked: loader.startPlayback(media.url)
            function remove() {
                if (media.url == "" || media.fileName == "") {
                    return
                }

                popup.open()
            }

            DeletePopup {
                id: popup
                page: postCaptureView
                item: image
                file: media.fileName
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
        onCurrentIndexChanged: view.currentIndex = currentIndex
        highlightFollowsCurrentItem: true
        orientation: ListView.Horizontal
        model: postCaptureModel
        visible: anchors.bottomMargin > -100
        interactive: view.interactive

        Behavior on anchors.bottomMargin {
            NumberAnimation { duration: 200 }
        }

        delegate: Rectangle {
            id: rectangle
            width: 120
            height: 120
            border.color: media.video ? "blue" : "white"
            border.width: 2
            color: "black"
            enabled: view.interactive

            scale: mouse.pressed ? 2 : 1
            z: scale > 1 ? 1 : 0

            transformOrigin: Item.Bottom

            Behavior on scale {
                NumberAnimation { duration: 200 }
            }

            MouseArea {
                id: mouse
                anchors.fill: parent
                onPressed: {
                    // If we use ListView.view then QtQuick 2.0 seems to break
                    // and mapToItem() returns a weird coordinate.
                    var x = mapToItem(thumbnails, mouse.x, mouse.y).x
                    if (x <= rectangle.width) {
                        rectangle.transformOrigin = Item.BottomLeft
                    } else if (x >= thumbnails.width - rectangle.width) {
                        rectangle.transformOrigin = Item.BottomRight
                    } else {
                        rectangle.transformOrigin = Item.Bottom
                    }
                }

                onClicked: {
                    if (thumbnails.currentIndex == index) {
                        if (view.currentItem) {
                            view.currentItem.load()
                        }
                    } else {
                        thumbnails.currentIndex = index
                    }
                }
            }

            QuillItem {
                id: thumbnail
                width: 116
                height: 116
                anchors.centerIn: parent
                url: media.url
                mimeType: media.mimeType
                displayLevel: QuillItem.DisplayLevelCropped
                priority: QuillItem.PriorityLow
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
        enabled: view.interactive

        Behavior on anchors.topMargin {
            NumberAnimation { duration: 200 }
        }

        tools: CameraToolBarTools {
            CameraToolIcon {
                iconSource: cameraTheme.shareIconId
                onClicked: {
                    if (!share.shareUrl(view.currentItem.url, view.currentItem.mime, view.currentItem.fileName)) {
                        showError(qsTr("Failed to launch share service"))
                    }
                }

                enabled: view.currentItem != null
            }

            CameraToolIcon {
                iconSource: cameraTheme.deleteIconId
                onClicked: view.currentItem.remove()
                enabled: view.currentItem != null
            }

            CameraToolIcon {
                iconSource: cameraTheme.galleryIconId
                onClicked: gallery.launchGallery()
            }

            CameraLabel {
                height: toolBar.height
                text: view.currentItem ? view.currentItem.fileName : ""
                width: 350
                font.pixelSize: 32
                font.bold: true
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    CameraShareHelper {
        id: share
        settings: platformSettings
    }

    GalleryHelper {
        id: gallery
        settings: platformSettings

        function launchGallery() {
            if (!gallery.launch(view.currentItem.url)) {
                showError(qsTr("Failed to launch gallery"))
            }
        }
    }

    DeleteHelper {
        id: deleteHelper
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
