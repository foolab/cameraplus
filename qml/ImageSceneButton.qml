// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Selector {
        id: button

        property alias value: scene.value

        iconSource: sceneIcon(scene.value);

        Scene {
                id: scene
                camera: cam
                // TODO: hardcoding
                value: Scene.Auto
        }

        function sceneIcon(val) {
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
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-auto.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-auto-selected.png"
                        controller: scene
                        value: Scene.Auto
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-macro.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-macro-selected.png"
                        controller: scene
                        value: Scene.Closeup
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-landscape.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-landscape-selected.png"
                        controller: scene
                        value: Scene.Landscape
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-portrait.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-portrait-selected.png"
                        controller: scene
                        value: Scene.Portrait
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-night.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-night-selected.png"
                        controller: scene
                        value: Scene.Night
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-sports.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-scene-sports-selected.png"
                        controller: scene
                        value: Scene.Sport
                }
        }
}
