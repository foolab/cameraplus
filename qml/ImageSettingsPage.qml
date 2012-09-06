// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Page {
        id: page
        property Camera cam: null

        property variant __wb: [
        [WhiteBalance.Auto, qsTr("Automatic"), "icon-m-camera-whitebalance-auto-selected"],
        [WhiteBalance.Sunset, qsTr("Sunny"), "icon-m-camera-whitebalance-sunny-selected"],
        [WhiteBalance.Cloudy, qsTr("Cloudy"), "icon-m-camera-whitebalance-cloudy-selected"],        [WhiteBalance.Flourescent, qsTr("Flourescent"), "icon-m-camera-whitebalance-fluorescent-selected"],
        [WhiteBalance.Tungsten, qsTr("Tungsten"), "icon-m-camera-whitebalance-tungsten-selected"],
        ]

        property variant __cf: [
        [ColorTone.Normal, qsTr("Off"), "icon-m-camera-no-filter-selected"],
        [ColorTone.GrayScale, qsTr("Black & white"), "icon-m-camera-filter-black-white-selected"],
        [ColorTone.Sepia, qsTr("Sepia"), "icon-m-camera-filter-sepia-selected"],
        [ColorTone.Vivid, qsTr("Vivid"), "icon-m-camera-filter-vivid-selected"],
        [ColorTone.Negative, qsTr("Negative"), "icon-m-camera-filter-negative-selected"],
        [ColorTone.Solarize, qsTr("Solarize"), "icon-m-camera-filter-solarize-selected"]
        ]

        function filterData(val, data, item) {
                var x = 0;
                var i = data.length;
                for (x = 0; x < i; x++) {
                        if (data[x][0] == val) {
                                return data[x][item];
                        }
                }
        }

        function wbIcon(wb) {
                return filterData(wb, __wb, 2);
        }

        function wbName(wb) {
                return filterData(wb, __wb, 1);
        }

        function cfIcon(cf) {
                return filterData(cf, __cf, 2);
        }

        function cfName(cf) {
                return filterData(cf, __cf, 1);
        }

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
                                        subtitle: wbName(settings.imageWhiteBalance);
                                        iconId: wbIcon(settings.imageWhiteBalance);
                                        onClicked: openFile("ImageWhiteBalancePage.qml");
                                }

                                ListItem {
                                        id: cf
                                        width: parent.width / 2
                                        title: qsTr("Color filter");
                                        subtitle: cfName(settings.imageColorFilter);
                                        iconId: cfIcon(settings.imageColorFilter);
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
