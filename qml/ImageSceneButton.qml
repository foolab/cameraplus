// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0
import "data.js" as Data

Selector {
        id: button

        property int value: settings.imageSceneMode

        iconSource: sceneIcon(scene.value);

        title: qsTr("Scene mode");

        function sceneIcon(val) {
                // TODO: use the js functions.
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
                        normalIcon: "image://theme/" + Data.ismIcon(value);
                        checkedIcon: "image://theme/" + Data.ismSelectedIcon(value);
                        savedValue: settings.imageSceneMode
                        onClicked: settings.imageSceneMode = value;
                        value: Scene.Auto
                }

                CheckButton {
                        normalIcon: "image://theme/" + Data.ismIcon(value);
                        checkedIcon: "image://theme/" + Data.ismSelectedIcon(value);
                        savedValue: settings.imageSceneMode
                        onClicked: settings.imageSceneMode = value;
                        value: Scene.Closeup
                }

                CheckButton {
                        normalIcon: "image://theme/" + Data.ismIcon(value);
                        checkedIcon: "image://theme/" + Data.ismSelectedIcon(value);
                        savedValue: settings.imageSceneMode
                        onClicked: settings.imageSceneMode = value;
                        value: Scene.Landscape
                }

                CheckButton {
                        normalIcon: "image://theme/" + Data.ismIcon(value);
                        checkedIcon: "image://theme/" + Data.ismSelectedIcon(value);
                        savedValue: settings.imageSceneMode
                        onClicked: settings.imageSceneMode = value;
                        value: Scene.Portrait
                }

                CheckButton {
                        normalIcon: "image://theme/" + Data.ismIcon(value);
                        checkedIcon: "image://theme/" + Data.ismSelectedIcon(value);
                        savedValue: settings.imageSceneMode
                        onClicked: settings.imageSceneMode = value;
                        value: Scene.Night
                }

                CheckButton {
                        normalIcon: "image://theme/" + Data.ismIcon(value);
                        checkedIcon: "image://theme/" + Data.ismSelectedIcon(value);
                        savedValue: settings.imageSceneMode
                        onClicked: settings.imageSceneMode = value;
                        value: Scene.Sport
                }
        }
}
