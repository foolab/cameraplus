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
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-whitebalance-auto.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-whitebalance-auto-selected.png"
                        controller: wb
                        value: WhiteBalance.Auto
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-whitebalance-cloudy.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-whitebalance-cloudy-selected.png"
                        controller: wb
                        value: WhiteBalance.Cloudy
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-whitebalance-sunny.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-whitebalance-sunny-selected.png"
                        controller: wb
                        value: WhiteBalance.Sunset
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-whitebalance-tungsten.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-whitebalance-tungsten-selected.png"
                        controller: wb
                        value: WhiteBalance.Tungsten
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-whitebalance-fluorescent.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-whitebalance-fluorescent-selected.png"
                        controller: wb
                        value: WhiteBalance.Flourescent
                }

        }
}
