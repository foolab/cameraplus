// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0
import CameraPlus 1.0
import "data.js" as Data

CameraPage {
        id: page

        controlsVisible: false
        policyMode: CameraResources.Image
        needsPipeline: true

        Rectangle {
                color: "black"
                width: parent.width
                height: row.height + title.height + 30
                anchors.bottom: toolBar.top
                opacity: 0.5

                SectionHeader {
                        id: title
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        text: qsTr("White balance");
                }

                Row {
                        id: row
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: 20
                        anchors.rightMargin: 20
                        anchors.top: title.bottom
                        anchors.topMargin: 10

                        IconButton {
                                width: parent.width / 5
                                normalIcon: "image://theme/" + Data.wbIcon(value);
                                checkedIcon: "image://theme/" + Data.wbSelectedIcon(value);
                                value: WhiteBalance.Auto
                                savedValue: settings.imageWhiteBalance
                                onClicked: settings.imageWhiteBalance = value;
                                text: Data.wbName(value);
                        }

                        IconButton {
                                width: parent.width / 5
                                normalIcon: "image://theme/" + Data.wbIcon(value);
                                checkedIcon: "image://theme/" + Data.wbSelectedIcon(value);
                                value: WhiteBalance.Sunset
                                savedValue: settings.imageWhiteBalance
                                onClicked: settings.imageWhiteBalance = value;
                                text: Data.wbName(value);
                        }

                        IconButton {
                                width: parent.width / 5
                                normalIcon: "image://theme/" + Data.wbIcon(value);
                                checkedIcon: "image://theme/" + Data.wbSelectedIcon(value);
                                value: WhiteBalance.Cloudy
                                savedValue: settings.imageWhiteBalance
                                onClicked: settings.imageWhiteBalance = value;
                                text: Data.wbName(value);
                        }

                        IconButton {
                                width: parent.width / 5
                                normalIcon: "image://theme/" + Data.wbIcon(value);
                                checkedIcon: "image://theme/" + Data.wbSelectedIcon(value);
                                value: WhiteBalance.Flourescent
                                savedValue: settings.imageWhiteBalance
                                onClicked: settings.imageWhiteBalance = value;
                                text: Data.wbName(value);
                        }

                        IconButton {
                                width: parent.width / 5
                                normalIcon: "image://theme/" + Data.wbIcon(value);
                                checkedIcon: "image://theme/" + Data.wbSelectedIcon(value);
                                value: WhiteBalance.Tungsten
                                savedValue: settings.imageWhiteBalance
                                onClicked: settings.imageWhiteBalance = value;
                                text: Data.wbName(value);
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
