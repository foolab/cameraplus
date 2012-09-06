// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Page {
        id: page
        property Camera cam: null

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
                                normalIcon: "image://theme/icon-m-camera-whitebalance-auto"
                                checkedIcon: "image://theme/icon-m-camera-whitebalance-auto-selected"
                                value: WhiteBalance.Auto
                                savedValue: settings.imageWhiteBalance
                                onClicked: settings.imageWhiteBalance = value;
                                text: qsTr("Automatic");
                        }

                        IconButton {
                                width: parent.width / 5
                                normalIcon: "image://theme/icon-m-camera-whitebalance-sunny"
                                checkedIcon: "image://theme/icon-m-camera-whitebalance-sunny-selected"
                                value: WhiteBalance.Sunset
                                savedValue: settings.imageWhiteBalance
                                onClicked: settings.imageWhiteBalance = value;
                                text: qsTr("Sunset");
                        }

                        IconButton {
                                width: parent.width / 5
                                normalIcon: "image://theme/icon-m-camera-whitebalance-cloudy"
                                checkedIcon: "image://theme/icon-m-camera-whitebalance-cloudy-selected"
                                value: WhiteBalance.Cloudy
                                savedValue: settings.imageWhiteBalance
                                onClicked: settings.imageWhiteBalance = value;
                                text: qsTr("Cloudy");
                        }

                        IconButton {
                                width: parent.width / 5
                                normalIcon: "image://theme/icon-m-camera-whitebalance-fluorescent"
                                checkedIcon: "image://theme/icon-m-camera-whitebalance-fluorescent-selected"
                                value: WhiteBalance.Flourescent
                                savedValue: settings.imageWhiteBalance
                                onClicked: settings.imageWhiteBalance = value;
                                text: qsTr("Flourescent");
                        }

                        IconButton {
                                width: parent.width / 5
                                normalIcon: "image://theme/icon-m-camera-whitebalance-tungsten"
                                checkedIcon: "image://theme/icon-m-camera-whitebalance-tungsten-selected"
                                value: WhiteBalance.Tungsten
                                savedValue: settings.imageWhiteBalance
                                onClicked: settings.imageWhiteBalance = value;
                                text: qsTr("Tungsten");
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
