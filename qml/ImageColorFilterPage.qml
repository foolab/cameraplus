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
                                normalIcon: "image://theme/" + Data.cfIcon(value);
                                checkedIcon: "image://theme/" + Data.cfSelectedIcon(value);
                                value: ColorTone.Normal
                                savedValue: settings.imageColorFilter
                                onClicked: settings.imageColorFilter = value;
                                text: Data.cfName(value);
                        }

                        IconButton {
                                width: parent.width / 6
                                normalIcon: "image://theme/" + Data.cfIcon(value);
                                checkedIcon: "image://theme/" + Data.cfSelectedIcon(value);
                                value: ColorTone.GrayScale
                                savedValue: settings.imageColorFilter
                                onClicked: settings.imageColorFilter = value;
                                text: Data.cfName(value);
                        }

                        IconButton {
                                width: parent.width / 6
                                normalIcon: "image://theme/" + Data.cfIcon(value);
                                checkedIcon: "image://theme/" + Data.cfSelectedIcon(value);
                                value: ColorTone.Sepia
                                savedValue: settings.imageColorFilter
                                onClicked: settings.imageColorFilter = value;
                                text: Data.cfName(value);
                        }

                        IconButton {
                                width: parent.width / 6
                                normalIcon: "image://theme/" + Data.cfIcon(value);
                                checkedIcon: "image://theme/" + Data.cfSelectedIcon(value);
                                value: ColorTone.Vivid
                                savedValue: settings.imageColorFilter
                                onClicked: settings.imageColorFilter = value;
                                text: Data.cfName(value);
                        }

                        IconButton {
                                width: parent.width / 6
                                normalIcon: "image://theme/" + Data.cfIcon(value);
                                checkedIcon: "image://theme/" + Data.cfSelectedIcon(value);
                                value: ColorTone.Negative
                                savedValue: settings.imageColorFilter
                                onClicked: settings.imageColorFilter = value;
                                text: Data.cfName(value);
                        }

                        IconButton {
                                width: parent.width / 6
                                normalIcon: "image://theme/" + Data.cfIcon(value);
                                checkedIcon: "image://theme/" + Data.cfSelectedIcon(value);
                                value: ColorTone.Solarize
                                savedValue: settings.imageColorFilter
                                onClicked: settings.imageColorFilter = value;
                                text: Data.cfName(value);
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
