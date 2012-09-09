// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1

Column {
        id: col
        spacing: 10

        Label {
                // TODO:
                text: qsTr("Camera settings");
        }

        SectionHeader {
                text: qsTr("Show captured content");
        }

        ButtonRow {
                anchors.horizontalCenter: parent.horizontalCenter
                Button { text: qsTr("Disabled"); }
                Button { text: qsTr("2 seconds"); }
                Button { text: qsTr("5 seconds"); }
                Button { text: qsTr("No timeout"); }
        }

        SectionHeader {
                text: qsTr("Creator name");
        }

        TextField {
                placeholderText: qsTr("Name or copyright");
                width: parent.width
        }

        Item {
                width: parent.width
                height: 40 // TODO: hardcoded

                Label {
                        id: gpsLabel
                        anchors.left: parent.left
                        text: qsTr("Use GPS");
                }

                Switch {
                        anchors.right: parent.right
                }
        }

        Item {
                width: parent.width
                height: 40 // TODO: hardcoded

                Label {
                        id: geotagsLabel
                        anchors.left: parent.left
                        text: qsTr("Use geotags");
                }

                Switch {
                        anchors.right: parent.right
                }
        }
}
