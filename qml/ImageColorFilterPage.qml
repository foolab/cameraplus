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
                        text: qsTr("Color filter");
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
                                width: parent.width / 6
                                normalIcon: "image://theme/icon-m-camera-no-filter"
                                checkedIcon: "image://theme/icon-m-camera-no-filter-selected"
                                value: ColorTone.Normal
                                savedValue: settings.imageColorFilter
                                onClicked: settings.imageColorFilter = value;
                                text: qsTr("Off");
                        }

                        IconButton {
                                width: parent.width / 6
                                normalIcon: "image://theme/icon-m-camera-filter-black-white"
                                checkedIcon: "image://theme/icon-m-camera-filter-black-white-selected"
                                value: ColorTone.GrayScale
                                savedValue: settings.imageColorFilter
                                onClicked: settings.imageColorFilter = value;
                                text: qsTr("Black & white");
                        }

                        IconButton {
                                width: parent.width / 6
                                normalIcon: "image://theme/icon-m-camera-filter-sepia"
                                checkedIcon: "image://theme/icon-m-camera-filter-sepia-selected"
                                value: ColorTone.Sepia
                                savedValue: settings.imageColorFilter
                                onClicked: settings.imageColorFilter = value;
                                text: qsTr("Sepia");
                        }

                        IconButton {
                                width: parent.width / 6
                                normalIcon: "image://theme/icon-m-camera-filter-vivid"
                                checkedIcon: "image://theme/icon-m-camera-filter-vivid-selected"
                                value: ColorTone.Vivid
                                savedValue: settings.imageColorFilter
                                onClicked: settings.imageColorFilter = value;
                                text: qsTr("Vivid");
                        }

                        IconButton {
                                width: parent.width / 6
                                normalIcon: "image://theme/icon-m-camera-filter-negative"
                                checkedIcon: "image://theme/icon-m-camera-filter-negative-selected"
                                value: ColorTone.Negative
                                savedValue: settings.imageColorFilter
                                onClicked: settings.imageColorFilter = value;
                                text: qsTr("Negative");
                        }

                        IconButton {
                                width: parent.width / 6
                                normalIcon: "image://theme/icon-m-camera-filter-solarize"
                                checkedIcon: "image://theme/icon-m-camera-filter-solarize-selected"
                                value: ColorTone.Solarize
                                savedValue: settings.imageColorFilter
                                onClicked: settings.imageColorFilter = value;
                                text: qsTr("Solarize");
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
