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

// QML QtGallery stuff is crap.
// Most of the ideas (and some code) for loading and displaying images are stolen from
// N9QmlPhotoPicker https://github.com/petrumotrescu/N9QmlPhotoPicker

// TODO: this is really basic.
// TODO: Seems losing resources in post capture will not be recovered from.

CameraPage {
        id: page

        controlsVisible: false
        policyMode: CameraResources.PostCapture
        needsPipeline: true

        onStatusChanged: {
                if (status == PageStatus.Active) {
                        cam.stop();
                }
        }

        Connections {
                // Unlikely that we need this.
                target: cam
                onIdleChanged: {
                        if (cam.idle && page.status == PageStatus.Active) {
                                cam.stop();
                        }
                }
        }

        Rectangle {
                color: "black"
                anchors.fill: parent
        }

        PathView {
                id: view
                anchors.fill: parent

                path: Path {
                        startX: 0
                        startY: view.height / 2
                        PathLine { x: view.width; y: view.height / 2 }
                }

                flickDeceleration: 999999 // Insanely high value to prevent panning multiple images
                preferredHighlightBegin: 0.5
                preferredHighlightEnd: 0.5
                highlightRangeMode: PathView.StrictlyEnforceRange
                pathItemCount: 1

                model: SparqlListModel {
                        // This is the exact query used by Harmattan gallery.
                        // Gallery prints it as part of its debugging when
                        // passing -output-level debug ;-)
                        query: "SELECT nie:url(?urn) AS ?url nfo:fileName(?urn) AS ?filename ?created nie:mimeType(?urn) AS ?mimetype ( EXISTS { ?urn nao:hasTag nao:predefined-tag-favorite . } ) AS ?favorite nfo:duration(?urn) AS ?duration ?urn \"false\"^^xsd:boolean AS ?isVirtual nfo:fileLastModified(?urn) as ?lastmod nfo:hasRegionOfInterest(?urn) as ?roi tracker:id(?urn) AS ?trackerid WHERE { ?urn rdf:type nfo:Visual ; tracker:available \"true\"^^xsd:boolean . OPTIONAL { ?urn nie:contentCreated ?created } . ?urn nfo:equipment \"urn:equipment:" + deviceInfo.manufacturer + ":" + deviceInfo.model + ":\" . } ORDER BY DESC (?created)"

                        connection: SparqlConnection {
                                id: connection
                                driver: "QTRACKER_DIRECT"
                                onStatusChanged: checkStatus(status)

                                function checkStatus(status) {
                                        if (status == SparqlConnection.Error) {
                                                console.log("Error = "+connection.errorString());
                                        }
                                }
                        }
                }

                delegate: PostCaptureItem {}
        }

        ToolBar {
                id: toolBar
                opacity: 0.8
                anchors.bottom: parent.bottom
                tools: ToolBarLayout {
                        id: layout
                        ToolIcon { iconId: "icon-m-toolbar-back"; onClicked: { cam.start(); pageStack.pop(); } }
                }
        }

        Rectangle {
                // TODO: transitions
                id: rect
                anchors.top: parent.top
                height: toolBar.height
                width: parent.width
                color: "black"
                opacity: toolBar.opacity
                visible: toolBar.visible
                Row {
                        anchors.fill: parent

                        Label {

                        }

                        Label {

                        }
                }
        }
}
