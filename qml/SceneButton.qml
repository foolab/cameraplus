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

        property alias sceneValue: scene.value

        function sceneIcon(val) {
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

        Scene {
                id: scene
                camera: cam
                // TODO: hardcoding
                value: Scene.Auto
        }

        Button {
                anchors.left: parent.left
                id: button
                width: 64
                height: 64
                opacity: 0.5
                onClicked: mouse.enabled = !mouse.enabled;
                checkable: true
                iconSource: sceneIcon(scene.value);
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
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-auto.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-auto-selected.png"
                        controller: scene
                        value: Scene.Auto
                        fader: mouse
                }
                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-macro.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-macro-selected.png"
                        controller: scene
                        value: Scene.Closeup
                        fader: mouse
                }
                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-landscape.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-landscape-selected.png"
                        controller: scene
                        value: Scene.Landscape
                        fader: mouse
                }
                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-portrait.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-portrait-selected.png"
                        controller: scene
                        value: Scene.Portrait
                        fader: mouse
                }
                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-night.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-night-selected.png"
                        controller: scene
                        value: Scene.Night
                        fader: mouse
                }
                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-sports.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-sports-selected.png"
                        controller: scene
                        value: Scene.Sport
                        fader: mouse
                }
        }
}
