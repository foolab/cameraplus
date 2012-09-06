// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1

Row {
        id: root

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 10

        property alias text: label.text

        Label {
                id: label
                verticalAlignment: Text.AlignVCenter
        }

        Rectangle {
                color: "darkgray"
                height: 2
                width: parent.width - label.width - 20
                anchors.verticalCenter: parent.verticalCenter
        }
}
