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

        ListView {
                // TODO: ListView does not loop and seems one has to use PathView.
                id: view
                anchors.fill: parent
                orientation: ListView.Horizontal
                snapMode: ListView.SnapOneItem
                cacheBuffer: view.width * 2

                model: SparqlListModel {
                        query: "SELECT nie:url(?urn) AS ?url tracker:id(?urn) AS ?trackerid nie:mimeType(?urn) AS ?mime WHERE { ?urn rdf:type nfo:Media .  ?urn nfo:equipment \"urn:equipment:Nokia:N950:\" ; tracker:available \"true\"^^xsd:boolean  }  ORDER BY DESC (?created)"

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

                        QuillItem {
                                source: url
                                mimeType: mime
                                width: view.width - 10
                                height: view.height
                                anchors.centerIn: parent
                                // TODO: does not work because of preloading
                                //                        onError: showError(qsTr("Failed to load image"));
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
