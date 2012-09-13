// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0
import QtSparql 1.0
import CameraPlus 1.0

// QML QtGallery stuff is crap.
// Most of the ideas (and some code) for loading and displaying images are stolen from
// N9QmlPhotoPicker https://github.com/petrumotrescu/N9QmlPhotoPicker

// TODO: this is really basic.

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

                // TODO: tap post capture and then immediately back and you can see the error
                // and the standby widget underneath it.
                delegate: Item {
                        width: view.width
                        height: view.height

                        Label {
                                width: view.width - 10
                                height: view.height
                                anchors.centerIn: parent
                                visible: item.error
                                text: qsTr("Failed to load preview");
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: 32
                        }

                        QuillItem {
                                id: item
                                source: url
                                mimeType: mimetype
                                width: view.width - 10
                                height: view.height
                                anchors.centerIn: parent
                        }
                }
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
}
