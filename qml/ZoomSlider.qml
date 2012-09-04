// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Slider {
        id: slider
        property alias camera: zoom.camera
        platformStyle: SliderStyle {
                // HACK
                handleBackground: " "
                handleBackgroundPressed: " "
        }

        Zoom {
                id: zoom
                value: slider.value
        }

        Connections {
                target: camera
                onModeChanged: slider.value = zoom.minimum;
        }

        orientation: Qt.Horizontal
        width: 500
        height: 50
        stepSize:0.1
        minimumValue: zoom.minimum
        maximumValue: zoom.maximum

        state: "hidden"
        states: [
        State {
                name: "visible"
                when: slider.pressed
                PropertyChanges { target: slider; opacity: 1.0 }
        },
        State {
                name: "hidden"
                when: !slider.pressed
                PropertyChanges { target: slider; opacity: 0.2 }
        }]

        transitions: Transition {
                to: "hidden"
                SequentialAnimation {
                        PauseAnimation { duration: 2000 }
                        NumberAnimation { target: slider; property: "opacity"; duration: 250 }
                }
        }
}
