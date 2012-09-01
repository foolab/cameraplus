// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Image {
        id: image
        anchors.fill: parent
        property alias animationRunning: animation.running

        visible: false
        cache: false
        fillMode: Image.PreserveAspectFit

        SequentialAnimation {
                id: animation
                PauseAnimation { duration: 500; alwaysRunToEnd: true }
                NumberAnimation { target: preview; property: "opacity"; from: 1; to: 0; duration: 250; alwaysRunToEnd: true }
        }

        function setPreview(preview) {
                animation.start();
                image.source = preview;
                image.visible = true;
        }
}
