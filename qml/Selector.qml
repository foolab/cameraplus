// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Button {
        id: button
        width: 64
        height: 64

        property alias widget: __widget.children
        property bool timerConstraints: false

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

                Item {
                        id: __widget
                        y: button.y
                        x: button.x + button.width + 20
                        visible: button.checked
                }
        }
}
