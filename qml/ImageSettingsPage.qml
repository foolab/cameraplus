// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Page {
        id: page

        // TODO:

        Flickable {
                id: flick
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: toolBar.top

                WhiteBalanceButton {
                        id: wb
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.topMargin: 20
                        anchors.leftMargin: 20
                }

                ColorToneButton {
                        id: ct
                        anchors.top: wb.bottom
                        anchors.left: parent.left
                        anchors.topMargin: 20
                        anchors.leftMargin: 20
                }
        }

        ToolBar {
                id: toolBar
                anchors.bottom: parent.bottom
                // TODO: transparent ?
//                height: layout.height
//                platformStyle: ToolBarStyle {
//                        background: ""
//                }

                tools: ToolBarLayout {
                        id: layout
                        ToolIcon { iconId: "icon-m-toolbar-back"; onClicked: pageStack.pop(); }
                }
        }
}
