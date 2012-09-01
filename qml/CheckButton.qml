// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1

Button {
        id: button
        property string normalIcon: ""
        property string checkedIcon: ""
        property QtObject controller: null
        property QtObject fader: null
        property int value: -1

        width: visible ? 56 : 0
        height: visible ? 56 : 0
        iconSource: !visible ? "" : controller.value == value ? checkedIcon : normalIcon
        onClicked: { controller.value = value; close(); }
}
