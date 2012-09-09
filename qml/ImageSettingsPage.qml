// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

import "data.js" as Data

Page {
        id: page
        property Camera cam: null

        Rectangle {
                color: "black"
                anchors.fill: parent
        }

        Flickable {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: toolBar.top
                anchors.margins: 10
                contentHeight: col.height

                Column {
                        id: col

                        width: parent.width
                        spacing: 10

                        Label {
                                // TODO:
                                text: qsTr("Image settings");
                        }

                        SectionHeader {
                                text: qsTr("Capture mode");
                        }

                        ButtonRow {
                                anchors.horizontalCenter: parent.horizontalCenter
                                Button { text: qsTr("Normal"); }
                                Button { text: qsTr("Self timer"); }
                                Button { text: qsTr("Fast capture"); }
                        }

                        Row {
                                width: parent.width

                                ListItem {
                                        id: wb
                                        width: parent.width / 2
                                        title: qsTr("White balance");
                                        subtitle: Data.wbName(settings.imageWhiteBalance);
                                        iconId: Data.wbSelectedIcon(settings.imageWhiteBalance);
                                        onClicked: openFile("ImageWhiteBalancePage.qml");
                                }

                                ListItem {
                                        id: cf
                                        width: parent.width / 2
                                        title: qsTr("Color filter");
                                        subtitle: Data.cfName(settings.imageColorFilter);
                                        iconId: Data.cfSelectedIcon(settings.imageColorFilter);
                                        onClicked: openFile("ImageColorFilterPage.qml");
                                }
                        }

                        SectionHeader {
                                text: qsTr("Self timer");
                        }

                        ButtonRow {
                                anchors.horizontalCenter: parent.horizontalCenter
                                Button { text: qsTr("2 seconds"); }
                                Button { text: qsTr("10 seconds"); }
                        }

                        SectionHeader {
                                text: qsTr("Light sensitivity");
                        }

                        ButtonRow {
                                anchors.horizontalCenter: parent.horizontalCenter
                                Button { text: qsTr("Automatic"); }
                                Button { text: qsTr("ISO 100"); }
                                Button { text: qsTr("ISO 200"); }
                                Button { text: qsTr("ISO 400"); }
                                Button { text: qsTr("ISO 800"); }
                        }

                        SectionHeader {
                                text: qsTr("Aspect ratio");
                        }

                        ButtonRow {
                                anchors.horizontalCenter: parent.horizontalCenter
                                Button { text: qsTr("16:9"); }
                                Button { text: qsTr("4:3"); }
                                Button { text: qsTr("3:2"); }
                        }

                        SectionHeader {
                                text: qsTr("Resolution");
                        }

                        ButtonRow {
                                anchors.horizontalCenter: parent.horizontalCenter
                                Button {}
                                Button {}
                                Button {}
                        }

                        CameraSettings {
                                anchors.horizontalCenter: parent.horizontalCenter
                        }
                }
        }

        ToolBar {
                id: toolBar
                anchors.bottom: parent.bottom
                tools: ToolBarLayout {
                        id: layout
                        ToolIcon { iconId: "icon-m-toolbar-back"; onClicked: pageStack.pop(); }
                }
        }
}
