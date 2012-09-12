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

Page {
        id: page
        property Camera cam: null

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
                        query: "SELECT nie:url(?urn) AS ?url tracker:id(?urn) AS ?trackerid nie:mimeType(?urn) AS ?mime WHERE { ?urn rdf:type nfo:Media .  ?urn nfo:equipment \"urn:equipment:" + deviceInfo.manufacturer + ":" + deviceInfo.model + ":\" ; tracker:available \"true\"^^xsd:boolean  OPTIONAL { ?urn nie:contentCreated ?created }   }  ORDER BY DESC (?created)"

                        connection: SparqlConnection {
                                id: connection
                                driver: "QTRACKER_DIRECT"
                                onStatusChanged: checkStatus(status)

                                function checkStatus(status) {
                                        if (status == SparqlConnection.Error)
                                        console.log("Error = "+connection.errorString());
                                }
                        }
                }

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
                                mimeType: mime
                                width: view.width - 10
                                height: view.height
                                anchors.centerIn: parent
                                visible: !item.error
                        }
                }
        }

        ToolBar {
                id: toolBar
                opacity: 0.8
                anchors.bottom: parent.bottom
                tools: ToolBarLayout {
                        id: layout
                        ToolIcon { iconId: "icon-m-toolbar-back"; onClicked: pageStack.pop(); }
                }
        }
}
