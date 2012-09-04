// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Selector {
        id: button

        property alias value: wb.value

        timerConstraints: true // Never trigger

        iconSource: wbIcon(wb.value);

        title: qsTr("White balance mode");

        WhiteBalance {
                id: wb
                camera: cam
                // TODO: hardcoding
                value: WhiteBalance.Auto
        }

        function wbIcon(val) {
                var x = row.children.length;
                var i = 0;
                for (i = 0; i < x; i++) {
                        if (row.children[i].value == val) {
                                return row.children[i].normalIcon;
                        }
                }
        }

        widget: Row {
                id: row
                height: button.checked ? 64 : 0
                width: button.checked ? (children.length * height) +  (children.length - 1) * spacing : 0
                spacing: 10

                Behavior on width {
                        // TODO: seems animation is not working
                        PropertyAnimation { duration: 250; }
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-whitebalance-auto"
                        checkedIcon: "image://theme/icon-m-camera-whitebalance-auto-selected"
                        controller: wb
                        value: WhiteBalance.Auto
                        doClose: false
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-whitebalance-cloudy"
                        checkedIcon: "image://theme/icon-m-camera-whitebalance-cloudy-selected"
                        controller: wb
                        value: WhiteBalance.Cloudy
                        doClose: false
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-whitebalance-sunny"
                        checkedIcon: "image://theme/icon-m-camera-whitebalance-sunny-selected"
                        controller: wb
                        value: WhiteBalance.Sunset
                        doClose: false
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-whitebalance-tungsten"
                        checkedIcon: "image://theme/icon-m-camera-whitebalance-tungsten-selected"
                        controller: wb
                        value: WhiteBalance.Tungsten
                        doClose: false
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-whitebalance-fluorescent"
                        checkedIcon: "image://theme/icon-m-camera-whitebalance-fluorescent-selected"
                        controller: wb
                        value: WhiteBalance.Flourescent
                        doClose: false
                }

        }
}
