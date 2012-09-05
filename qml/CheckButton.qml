// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1

Button {
        id: button
        property string normalIcon: ""
        property string checkedIcon: ""
        property QtObject fader: null
        property int value: -1
        property bool doClose: true
        property int savedValue: -1

        width: visible ? 56 : 0
        height: visible ? 56 : 0
        iconSource: !visible ? "" : savedValue == value ? checkedIcon : normalIcon

        onClicked: {
                if (doClose) {
                        close();
                }
        }
}
