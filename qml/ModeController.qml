// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Item {
        id: controller
        property int mode: settings.mode
        property Camera cam: null
        property Item dimmer: null
        property alias animationRunning: animation.running

        SequentialAnimation {
                id: animation
                property int mode: 0

                function setMode() {
                        cam.mode = mode;
                }

                NumberAnimation { target: dimmer; property: "opacity"; from: 0; to: 1; duration: 150; alwaysRunToEnd: true }

                PauseAnimation { duration: 50 }

                ParallelAnimation {
                        ScriptAction { script: animation.setMode(); }
                        PauseAnimation { duration: 200 }
                }

                PauseAnimation { duration: 50 }

                NumberAnimation { target: dimmer; property: "opacity"; from: 1; to: 0; duration: 150; alwaysRunToEnd: true }
        }

        onModeChanged: {
                set(mode);
        }

        function set(newMode) {
                if (cam.mode == newMode) {
                        return;
                }

                animation.mode = newMode;

                animation.start();
        }
}
