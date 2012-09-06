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

                Column {
                        width: parent.width

                        Label {
                                // TODO:
                                text: qsTr("Image settings");
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
