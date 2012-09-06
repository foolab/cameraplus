// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1

Item {
        id: root
        width: Math.max(150, Math.max(label.width, icon.width) + 20);
        height: 120

        property string normalIcon: ""
        property string checkedIcon: ""
        property int value: -1
        property int savedValue: -1

        property alias iconSource: icon.source
        property alias text: label.text

        signal clicked

        MouseArea {
                anchors.fill: parent
                onClicked: root.clicked();
        }

        Image {
                id: icon

                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.topMargin: 10
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                source: root.value == root.savedValue ? checkedIcon : normalIcon
                fillMode: Image.PreserveAspectFit
        }

        Label {
                id: label
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.top: icon.bottom
                anchors.bottomMargin: 10
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                anchors.topMargin: 10
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.NoWrap
                font.pixelSize: 22
        }
}
