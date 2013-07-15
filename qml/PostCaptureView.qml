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

import QtQuick 1.1
import CameraPlus 1.0
import com.nokia.meego 1.1
import QtCamera 1.0

Item {
    property bool pressed: view.currentItem ? view.currentItem.playing : false
    property int policyMode: settings.mode == Camera.VideoMode ? CameraResources.Video
        : CameraResources.Image
    property bool available: view.currentItem ? view.currentItem.itemData.available : false

    Component.onCompleted: postCaptureModel.reload()

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
        // TODO: this should not be active all the time
        id: postCaptureModel
        manufacturer: deviceInfo.manufacturer
        model: deviceInfo.model
        onError: {
            console.log("Error populating model " + msg)
            showError(qsTr("Failed to load captures"))
        }
    }

    Timer {
        id: hideTimer
        running: false
        interval: 3000
    }

    CameraToolBar {
        id: toolBar
        expanded: true
        manualBack: true
        anchors.bottom: parent.bottom
        anchors.bottomMargin: show ? 20 : -1 * (height + 20)
        anchors.left: parent.left
        anchors.leftMargin: 20
        opacity: 0.8
// TODO: hide back button
        property bool show: deleteDialog.status == DialogStatus.Open ||
            deleteDialog.status == DialogStatus.Opening ||
            hideTimer.running || menu.status == DialogStatus.Open ||
            menu.status == DialogStatus.Opening ||
            (view.currentItem && view.currentItem.error) && !view.currentItem.playing

// TODO:
//        onClicked: pageStack.pop()

        Behavior on anchors.bottomMargin {
            PropertyAnimation { duration: 200; }
        }

        items: [
            ToolIcon {
                iconId: !available ? "icon-m-toolbar-favorite-mark-dimmed-white" : view.currentItem.itemData.favorite ? "icon-m-toolbar-favorite-mark-white" : "icon-m-toolbar-favorite-unmark-white"
                onClicked: {
                    addOrRemoveFavorite()
                    restartTimer()
                }
            },
            ToolIcon {
                iconId: available ? "icon-m-toolbar-share-white" : "icon-m-toolbar-share-dimmed-white"
                onClicked: {
                    shareCurrentItem()
                    restartTimer()
                }
            },
            ToolIcon {
                iconId: available ? "icon-m-toolbar-delete-white" : "icon-m-toolbar-delete-dimmed-white"
                onClicked: {
                    deleteCurrentItem()
                    restartTimer()
                }
            },
            ToolIcon {
                iconId: "icon-m-toolbar-view-menu-white"
                onClicked: {
                    menu.open()
                    restartTimer()
                }
            }
        ]
    }

    QueryDialog {
        id: deleteDialog
        titleText: qsTr("Delete item?");
        acceptButtonText: qsTr("Yes");
        rejectButtonText: qsTr("No");

        onStatusChanged: restartTimer()

        onAccepted: {
            if (!remove.remove(view.currentItem.itemData.url)) {
                showError(qsTr("Failed to delete item"))
            } else {
                postCaptureModel.remove(view.currentItem.itemData);
            }
        }

        DeleteHelper {
            id: remove
        }
    }

    Menu {
        id: menu
        onStatusChanged: restartTimer()

        MenuLayout {
            MenuItem {
                text: qsTr("Captures in gallery")
                onClicked: launchGallery()
            }

            MenuItem {
                text: qsTr("View in gallery")
                enabled: available
                onClicked: showInGallery()
            }
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
            PropertyAnimation { duration: 200; }
        }

        Flow {
            width: parent.width - 40
            x: 20
            height: parent.height

            Label {
                text: view.currentItem ? view.currentItem.itemData.title : ""
                width: parent.width / 2
                height: parent.height
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }

            Label {
                text: view.currentItem ? view.currentItem.itemData.created : ""
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

    function showInGallery() {
        if (!available) {
            return
        }

        if (!gallery.show(view.currentItem.itemUrl)) {
            showError(qsTr("Failed to launch gallery"))
        }
    }

    function deleteCurrentItem() {
        if (!available) {
            return
        }

        deleteDialog.message = view.currentItem.itemData.fileName
        deleteDialog.open()
    }

    function shareCurrentItem() {
        if (!available) {
            return
        }

        if (!share.share(view.currentItem.itemData.url)) {
            showError(qsTr("Failed to launch share service"))
        }
    }

    function addOrRemoveFavorite() {
        if (!available) {
            return
        }

        if (view.currentItem.itemData.favorite) {
            if (!trackerStore.removeFromFavorites(view.currentItem.itemData.url)) {
                showError(qsTr("Failed to remove favorite"))
            } else {
                view.currentItem.itemData.favorite = false
            }
        } else {
            if (!trackerStore.addToFavorites(view.currentItem.itemData.url)) {
                showError(qsTr("Failed to add favorite"))
            } else {
                view.currentItem.itemData.favorite = true
            }
        }
    }

    function restartTimer() {
        hideTimer.restart()
    }
}
