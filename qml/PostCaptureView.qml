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

// TODO: qrc:/qml/PostCaptureView.qml:104:5: QML CameraToolBar: Binding loop detected for property "height"

Item {
    id: postCaptureView

    property Camera camera: null
    property bool pressed: view.currentItem ? view.currentItem.playing : false
    property int policyMode: view.currentItem && view.currentItem.playing ?
        CameraResources.Player : settings.mode == Camera.VideoMode ? CameraResources.Video :
        CameraResources.Image

    property bool isCurrent: mainView.currentIndex == 2 && !mainView.moving
    property bool inCameraMode: root.inCaptureMode && !mainView.moving

    onIsCurrentChanged: {
        if (isCurrent) {
            postCaptureModel.reload()
        }
    }

    onInCameraModeChanged: {
        if (inCameraMode) {
            postCaptureModel.clear()
        }
    }

    Connections {
        target: view.currentItem
        onPlayingChanged: {
            if (view.currentItem.playing) {
                hideTimer.running = false
            }
        }
    }

    ShareHelper {
        id: share
        settings: platformSettings
    }

    GalleryHelper {
        id: gallery
        settings: platformSettings
    }

    ListView {
        id: view
        anchors.fill: parent
        snapMode: ListView.SnapOneItem
        cacheBuffer: height * 3
        model: postCaptureModel
        highlightRangeMode: ListView.StrictlyEnforceRange
        interactive: view.currentItem && view.currentItem.playing ? false : true

        delegate: PostCaptureItem {
            width: view.width
            height: view.height
            onClicked: hideTimer.running = !hideTimer.running
        }

        onFlickingChanged: {
            if (flicking && hideTimer.running) {
                restartTimer()
            }
        }
    }

    PostCaptureModel {
        id: postCaptureModel
        imagePath: platformSettings.imagePath
        videoPath: platformSettings.videoPath
    }

    Timer {
        id: hideTimer
        running: false
        interval: 3000
    }

    CameraToolBar {
        id: toolBar
        expanded: true
        hideBack: true
        anchors.bottom: parent.bottom
        anchors.bottomMargin: show ? 20 : -1 * (height + 20)
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        opacity: 0.8

        property bool show: deleteDialog.isOpen || deleteDialog.isOpening ||
            hideTimer.running ||
            (view.currentItem != null && view.currentItem.error) && !view.currentItem.playing

        Behavior on anchors.bottomMargin {
            PropertyAnimation { duration: view.currentItem && view.currentItem.playing ? 0 : 200 }
        }

        tools: CameraToolBarTools {
            CameraToolIcon {
                iconSource: cameraTheme.shareIconId
                onClicked: {
                    shareCurrentItem()
                    restartTimer()
                }
            }

            CameraToolIcon {
                iconSource: cameraTheme.deleteIconId
                onClicked: {
                    deleteCurrentItem()
                    restartTimer()
                }
            }

            CameraToolIcon {
                iconSource: cameraTheme.galleryIconId
                onClicked: {
                    launchGallery()
                    restartTimer()
                }
            }
        }
    }

    CameraQueryDialog {
        id: deleteDialog
        titleText: qsTr("Delete item?");
        acceptButtonText: qsTr("Yes");
        rejectButtonText: qsTr("No");

        onStatusChanged: restartTimer()

        onAccepted: {
            if (!remove.remove(view.currentItem.itemUrl)) {
                showError(qsTr("Failed to delete item"))
            } else {
                view.model.remove(view.currentItem.itemUrl)
            }
        }

        DeleteHelper {
            id: remove
        }
    }

    Rectangle {
        opacity: toolBar.opacity
        anchors.top: parent.top
        anchors.topMargin: toolBar.show ? 20 : -1 * (height + 20)
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        visible: toolBar.visible
        height: screen.isPortrait ? toolBar.height * 2 : toolBar.height
        color: toolBar.color
        border.color: toolBar.border.color
        radius: toolBar.radius

        Behavior on anchors.topMargin {
            PropertyAnimation { duration: view.currentItem && view.currentItem.playing ? 0 : 200 }
        }

        Flow {
            width: parent.width - 40
            x: 20
            height: parent.height

            CameraLabel {
                text: view.currentItem ? view.currentItem.itemTitle : ""
                width: parent.width / 2
                height: parent.height
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }

            CameraLabel {
                text: view.currentItem ? view.currentItem.itemCreated : ""
                width: parent.width / 2
                height: parent.height
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
            }
        }
    }

    function launchGallery() {
        if (!gallery.launch()) {
            showError(qsTr("Failed to launch gallery"))
        }
    }

    function deleteCurrentItem() {
        if (view.currentItem == null) {
            return
        }

        deleteDialog.message = view.currentItem.itemFileName
        deleteDialog.open()
    }

    function shareCurrentItem() {
        if (view.currentItem != null && !share.share(view.currentItem.itemUrl)) {
            showError(qsTr("Failed to launch share service"))
        }
    }

    function restartTimer() {
        hideTimer.restart()
    }

    function policyLost() {
        if (view.currentItem && view.currentItem.playing) {
            view.currentItem.stopPlayback()
        }
    }
}
