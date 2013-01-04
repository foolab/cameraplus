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
import QtCamera 1.0
import CameraPlus 1.0

// TODO: losing resources while playback won't show an error

CameraPage {
        id: page

        controlsVisible: false
        policyMode: CameraResources.PostCapture
        needsPipeline: false
        standbyVisible: !Qt.application.active

        property Item currentItem: null
        property bool available: currentItem ? currentItem.itemData.available : false

        Component.onCompleted: postCaptureModel.reload();

        function launchGallery() {
                if (!gallery.launch()) {
                        showError(qsTr("Failed to launch gallery"));
                }
        }

        function showInGallery() {
                if (!available) {
                        return;
                }

                if (!gallery.show(currentItem.itemUrl)) {
                        showError(qsTr("Failed to launch gallery"));
                }
        }

        Menu {
                id: menu
                onStatusChanged: page.restartTimer();

                MenuLayout {
                        MenuItem {text: qsTr("Captures in gallery"); onClicked: launchGallery(); }
                        MenuItem {text: qsTr("View in gallery"); enabled: available; onClicked: showInGallery(); }
                }
        }

        function deleteCurrentItem() {
                if (!available) {
                        return;
                }

                deleteDialog.message = currentItem.itemData.fileName;
                deleteDialog.open();
        }

        QueryDialog {
                id: deleteDialog
                titleText: qsTr("Delete item?");
                acceptButtonText: qsTr("Yes");
                rejectButtonText: qsTr("No");

                onStatusChanged: page.restartTimer();

                onAccepted: {
                        if (!remove.remove(currentItem.itemData.url)) {
                                showError(qsTr("Failed to delete item"));
                        }
                        else {
                                postCaptureModel.remove(currentItem.itemData);
                        }
                }

                DeleteHelper {
                        id: remove
                }
        }

        function shareCurrentItem() {
                if (!available) {
                        return;
                }

                if (!share.share(currentItem.itemData.url)) {
                                showError(qsTr("Failed to launch share service"));
                }
        }

        function addOrRemoveFavorite() {
                if (!available) {
                        return;
                }

                if (currentItem.itemData.favorite) {
                        if (!trackerStore.removeFromFavorites(currentItem.itemData.url)) {
                                showError(qsTr("Failed to remove favorite"));
                        }
                        else {
                                currentItem.itemData.favorite = false;
                        }
                }
                else {
                        if (!trackerStore.addToFavorites(currentItem.itemData.url)) {
                                showError(qsTr("Failed to add favorite"));
                        }
                        else {
                                currentItem.itemData.favorite = true;
                        }
                }
        }

        ShareHelper {
                id: share
        }

        GalleryHelper {
                id: gallery
        }

        Rectangle {
                color: "black"
                anchors.fill: parent
        }

        PathView {
                id: view
                anchors.fill: parent

                path: Path {
                        startX: - view.width
                        startY: view.height / 2
                        PathLine { x: view.width * 2; y: view.height / 2 }
                }

                // Insanely high value to prevent panning multiple images
                flickDeceleration: 999999

                preferredHighlightBegin: 0.5
                preferredHighlightEnd: 0.5
                highlightRangeMode: PathView.StrictlyEnforceRange
                pathItemCount: 3

                model: PostCaptureModel {
                        id: postCaptureModel
                        manufacturer: deviceInfo.manufacturer
                        model: deviceInfo.model
                        onError: {
                                console.log("Error populating model " + msg);
                                showError(qsTr("Failed to load captures"));
                        }
                }

                Label {
                        // TODO: Hide this when we have no items
                        text: qsTr("No captures available");
                        anchors.centerIn: parent
                        font.pixelSize: 36
                        visible: currentItem == null
                }

                delegate: PostCaptureItem {
                        width: view.width
                        height: view.height
                        onClicked: hideTimer.running = !hideTimer.running;
                }
        }

        function restartTimer() {
                hideTimer.running = true;
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

                property bool show: deleteDialog.status == DialogStatus.Open || deleteDialog.status == DialogStatus.Opening || hideTimer.running || menu.status == DialogStatus.Open || menu.status == DialogStatus.Opening || (currentItem && currentItem.error)

                onClicked: pageStack.pop();

                Behavior on anchors.bottomMargin {
                        PropertyAnimation { duration: 200; }
                }

                items: [
                        ToolIcon { iconId: !available ? "icon-m-toolbar-favorite-mark-dimmed-white" : currentItem.itemData.favorite ? "icon-m-toolbar-favorite-mark-white" : "icon-m-toolbar-favorite-unmark-white"; onClicked: { addOrRemoveFavorite(); page.restartTimer(); } },
                        ToolIcon { iconId: available ? "icon-m-toolbar-share-white" : "icon-m-toolbar-share-dimmed-white"; onClicked: { shareCurrentItem(); page.restartTimer(); } },
                        ToolIcon { iconId: available ? "icon-m-toolbar-delete-white" : "icon-m-toolbar-delete-dimmed-white"; onClicked: { deleteCurrentItem(); page.restartTimer(); } },
                        ToolIcon { iconId: "icon-m-toolbar-view-menu-white"; onClicked: { menu.open(); page.restartTimer(); } }
                ]
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
                height: toolBar.height
                color: toolBar.color
                border.color: toolBar.border.color
                radius: toolBar.radius

                Behavior on anchors.topMargin {
                        PropertyAnimation { duration: 200; }
                }

                Label {
                        text: currentItem ? currentItem.itemData.title : ""
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        font.bold: true
                        verticalAlignment: Text.AlignVCenter
                }

                Label {
                        text: currentItem ? currentItem.itemData.created : ""
                        font.bold: true
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        anchors.rightMargin: 20
                        verticalAlignment: Text.AlignVCenter
                }
        }
}
