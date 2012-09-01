// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Item {
        id: controller
        property int mode: Camera.ImageMode
        property Camera cam: null
        property Item dimmer: null
        property alias animationRunning: animation.running

        SequentialAnimation {
                id: animation
                property int mode: 0

                function setMode() {
                        cam.mode = mode;
                        controller.mode = mode;
                }

                NumberAnimation { target: dimmer; property: "opacity"; from: 0; to: 1; duration: 250; alwaysRunToEnd: true }
                ParallelAnimation {
                        ScriptAction { script: animation.setMode(); }
                        PauseAnimation { duration: 100 }
                }

                NumberAnimation { target: dimmer; property: "opacity"; from: 1; to: 0; duration: 250; alwaysRunToEnd: true }
        }

        function set(newMode) {
                if (mode == newMode) {
                        return;
                }

                animation.start();

                animation.mode = newMode;
        }
}
