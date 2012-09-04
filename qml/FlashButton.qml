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
                        normalIcon: "image://theme/icon-m-camera-flash-auto"
                        checkedIcon: "image://theme/icon-m-camera-flash-auto-pressed"
                        controller: flash
                        value: Flash.Auto
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-flash-always"
                        checkedIcon: "image://theme/icon-m-camera-flash-always-pressed"
                        controller: flash
                        value: Flash.On
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-flash-off"
                        checkedIcon: "image://theme/icon-m-camera-flash-off-pressed"
                        controller: flash
                        value: Flash.Off
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-flash-red-eye"
                        checkedIcon: "image://theme/icon-m-camera-flash-red-eye-pressed"
                        controller: flash
                        value: Flash.RedEye
                }
        }
}
