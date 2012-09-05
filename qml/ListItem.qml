// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1

// This mimics MListItem

Rectangle {
        signal clicked

        property alias title: title.text
        property alias subtitle: subtitle.text

        property string iconId: ""

        height: 100

        id: button
        color: mouse.pressed ? "#464646" : "black"

        MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: button.clicked();
        }

        Image {
                id: icon
                anchors.top: button.top
                anchors.bottom: button.bottom
                anchors.left: button.left
                width: height
                height: button.height
                anchors.margins: 10
                source: button.iconId != "" ? "image://theme/" + button.iconId : ""
        }

        Label {
                id: title
                anchors.top: button.top
                anchors.left: icon.right
                anchors.right: button.right
                anchors.margins: 10
                font.bold: true
        }

        Label {
                id: subtitle
                anchors.top: title.bottom
                anchors.bottom: button.bottom
                anchors.left: icon.right
                anchors.right: button.right
                anchors.margins: 10
                platformStyle: LabelStyle {
                        textColor: "lightgray"
                }
        }
}
