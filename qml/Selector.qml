// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Button {
        id: button
        width: 56
        height: 56

        opacity: 0.5

        property alias widget: __widget.children
        property bool timerConstraints: false
        property alias title: label.text

        checkable: true

        function close() {
                button.checked = false;
        }

        Timer {
                interval: 2000
                running: mouse.enabled && !button.timerConstraints
                repeat: false
                onTriggered: button.close();
        }

        MouseArea {
                id: mouse
                parent: button.parent
                anchors.fill: parent
                enabled: button.checked
                onClicked: button.close();

                Label {
                        id: label
                        y: button.y
                        x: button.x + button.width + 20
                        visible: button.checked
                }

                Item {
                        id: __widget
                        anchors.top: label.bottom
                        anchors.topMargin: 10
//                        y: button.y
                        x: button.x + button.width + 20
                        visible: button.checked
                }
        }
}
