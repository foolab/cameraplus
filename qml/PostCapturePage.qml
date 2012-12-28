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
import QtSparql 1.0
import CameraPlus 1.0

// TODO: losing resources while playback won't show an error
// TODO: show something if we have no files.
// TODO: favorites
// TODO: menu
CameraPage {
        id: page

        controlsVisible: false
        policyMode: CameraResources.PostCapture
        needsPipeline: false
        standbyVisible: !Qt.application.active

        property Item currentItem: null
        property bool available: currentItem ? currentItem.itemAvailable : false

        function parseDate(str) {
                var parts = str.split('T');
                var dates = parts[0].split('-');
                var times = parts[1].split(':');
                return new Date(dates[0], dates[1], dates[2], times[0], times[1], times[2]);
        }

        function deleteCurrentItem() {
                if (!available) {
                        return;
                }

                deleteDialog.message = currentItem.fileName;
                deleteDialog.open();
        }

        QueryDialog {
                id: deleteDialog
                titleText: qsTr("Delete item?");
                acceptButtonText: qsTr("Yes");
                rejectButtonText: qsTr("No");
                onAccepted: {
                        // TODO: delete file, remove from model and move to next item
                }
        }

        function shareCurrentItem() {
                if (!available) {
                        return;
                }

                share.share(currentItem.itemUrl);
        }

        ShareHelper {
                id: share
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

                flickDeceleration: 999999 // Insanely high value to prevent panning multiple images
                preferredHighlightBegin: 0.5
                preferredHighlightEnd: 0.5
                highlightRangeMode: PathView.StrictlyEnforceRange
                pathItemCount: 3

                model: SparqlListModel {
                        query: 'SELECT rdf:type(?urn) AS ?type nie:url(?urn) AS ?url nie:contentCreated(?urn) AS ?created nie:title(?urn) AS ?title nfo:fileName(?urn) AS ?filename nie:mimeType(?urn) AS ?mimetype tracker:available(?urn) AS ?available nfo:fileLastModified(?urn) as ?lastmod tracker:id(?urn) AS ?trackerid  (EXISTS { ?urn nao:hasTag nao:predefined-tag-favorite }) AS ?favorite WHERE { ?urn nfo:equipment "urn:equipment:' + deviceInfo.manufacturer + ':' + deviceInfo.model + ':" .  {?urn a nfo:Video} UNION {?urn a nfo:Image}} ORDER BY DESC(?created)'

                        connection: SparqlConnection {
                                id: connection
                                driver: "QTRACKER_DIRECT"
                                onStatusChanged: checkStatus(status)

                                function checkStatus(status) {
                                        if (status == SparqlConnection.Error) {
                                                // TODO: report error
                                                console.log("Error = " + connection.errorString());
                                        }
                                }
                        }
                }

                delegate: PostCaptureItem {
                        width: view.width - 10
                        height: view.height
                }
        }

        ToolBar {
                id: toolBar
                opacity: 0.8
                anchors.bottom: parent.bottom
                tools: ToolBarLayout {
                        id: layout
                        ToolIcon { iconId: "icon-m-toolbar-back-white"; onClicked: { pageStack.pop(); } }
                        ToolIcon { iconId: available ? "icon-m-toolbar-favorite-mark-white" : "icon-m-toolbar-favorite-mark-dimmed-white"}
                        ToolIcon { iconId: available ? "icon-m-toolbar-share-white" : "icon-m-toolbar-share-dimmed-white"; onClicked: shareCurrentItem(); }
                        ToolIcon { iconId: available ? "icon-m-toolbar-delete-white" : "icon-m-toolbar-delete-dimmed-white"; onClicked: deleteCurrentItem(); }
                        ToolIcon { iconId: "icon-m-toolbar-view-menu-white" }
                }
        }

        ToolBar {
                opacity: toolBar.opacity
                anchors.top: parent.top
                visible: toolBar.visible

                tools: ToolBarLayout {
                        Label {
                                text: currentItem ? currentItem.itemTitle : ""
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                anchors.left: parent.left
                                font.bold: true
                                verticalAlignment: Text.AlignVCenter
                        }

                        Label {
                                text: currentItem ? Qt.formatDateTime(parseDate(currentItem.creationDate)) : ""
                                font.bold: true
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                anchors.right: parent.right
                                verticalAlignment: Text.AlignVCenter
                        }
                }
        }
}
