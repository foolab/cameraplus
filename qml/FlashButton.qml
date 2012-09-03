// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Selector {
        id: button

        property alias value: flash.value

        iconSource: flashIcon(flash.value);

        title: qsTr("Flash mode");

        Flash {
                id: flash
                camera: cam
                // TODO: hardcoding
                value: Flash.Auto
        }

        function flashIcon(val) {
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
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-flash-auto.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-flash-auto-pressed.png"
                        controller: flash
                        value: Flash.Auto
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-flash-always.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-flash-always-pressed.png"
                        controller: flash
                        value: Flash.On
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-flash-off.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-flash-off-pressed.png"
                        controller: flash
                        value: Flash.Off
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-flash-red-eye.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-flash-red-eye-pressed.png"
                        controller: flash
                        value: Flash.RedEye
                }
        }
}
