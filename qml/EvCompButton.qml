// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Selector {
        id: button

        property alias value: evComp.value

        iconSource: evComp.value == 0 ? "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-manual-exposure.png" : ""
        text: evComp.value == 0 ? "" : evComp.value.toFixed(1);
        font.pixelSize: 19
        timerConstraints: slider.pressed

        title: qsTr("Exposure compensation");

        EvComp {
                id: evComp
                camera: cam

                // TODO: hardcoding
                value: slider.value
        }

        widget: Slider {
                id: slider
                width: 500
                orientation: Qt.Horizontal
                minimumValue: evComp.minimum
                maximumValue: evComp.maximum
                value: evComp.value
                stepSize: 0.1
                Component.onCompleted: { slider.value = evComp.value; }
        }
}
