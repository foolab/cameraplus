// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

MouseArea {
        id: mouse
        anchors.fill: parent
        enabled: false
        onClicked: enabled = !enabled;
        onEnabledChanged: button.checked = enabled;

        function flashIcon(val) {
                var x = row.children.length;
                var i = 0;
                for (i = 0; i < x; i++) {
                        if (row.children[i].value == val) {
                                return row.children[i].normalIcon;
                        }
                }
        }

        Timer {
                interval: 2000
                running: mouse.enabled
                repeat: false
                onTriggered: mouse.enabled = !mouse.enabled
        }

        Flash {
                id: flash
                camera: cam
                // TODO: hardcoding
                value: Flash.Auto
        }

        Button {
                anchors.left: parent.left
                id: button
                width: 64
                height: 64
                opacity: 0.5
                onClicked: mouse.enabled = !mouse.enabled;
                checkable: true
                iconSource: flashIcon(flash.value);
        }

        Row {
                id: row
                height: mouse.enabled ? 64 : 0
                width: mouse.enabled ? (children.length * height) +  (children.length - 1) * spacing : 0
                anchors.left: button.right
                anchors.leftMargin: 20
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
                        fader: mouse
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-flash-always.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-flash-always-pressed.png"
                        controller: flash
                        value: Flash.On
                        fader: mouse
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-flash-off.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-flash-off-pressed.png"
                        controller: flash
                        value: Flash.Off
                        fader: mouse
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-flash-red-eye.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-flash-red-eye-pressed.png"
                        controller: flash
                        value: Flash.RedEye
                        fader: mouse
                }
        }
}
