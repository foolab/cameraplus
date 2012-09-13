// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0
import CameraPlus 1.0

Page {
        id: page

        property alias standbyWidget: standby

        property bool needsPipeline: true
        property int policyMode: CameraResources.None

        Component.onCompleted: {
                if (platformWindow.active && needsPipeline) {
                        resourcePolicy.acquire(page.policyMode);
                }
        }

        onStatusChanged: {
                if (platformWindow.active && status == PageStatus.Activating) {
                        resourcePolicy.acquire(page.policyMode);
                }
        }
/*
        onStatusChanged: {
                if (status == PageStatus.Active && !page.needsPipeline) {
                        cam.stop();
                }
        }
*/

        onPolicyModeChanged: {
                if (platformWindow.active) {
                        resourcePolicy.acquire(page.policyMode);
                }
        }

        function handlePipeline() {
                if (!platformWindow.active) {
                        // TODO: force if we lost resources ?
                        cam.stop();
                }
                else if (resourcePolicy.acquired && page.needsPipeline && !cam.running) {
                        // TODO: error
                        cam.start();
                }
                else if (!resourcePolicy.acquired) {
                        // TODO: force
                        cam.stop();
                }
        }

        Connections {
                target: resourcePolicy
                onAcquiredChanged: handlePipeline();
        }

        Connections {
                target: platformWindow
                onActiveChanged: {
                        if (!platformWindow.active) {
                                // This is a noop if camera is not
                                // idle so calling it will not hurt
                                if (cam.stop()) {
                                        resourcePolicy.acquire(CameraResources.None);
                                }
                        }
                        else if (page.needsPipeline) {
                                resourcePolicy.acquire(page.policyMode);
                        }
                }
        }

        Connections {
                target: cam
                onIdleChanged: {
                        if (cam.idle && !platformWindow.active) {
                                cam.stop();
                                resourcePolicy.acquire(CameraResources.None);
                        }
/*
                        else if (cam.idle && !page.needsPipeline) {
                                cam.stop();
                        }
*/
                }
        }

        Rectangle {
                // TODO: color
                // TODO: fade out transition
                // TODO: there is a toolbar visible on the first startup
                id: standby
                color: "black"
                anchors.fill: parent
                visible: !platformWindow.active || !cam.running || !resourcePolicy.acquired
                Image {
                        source: "image://theme/icon-l-camera-standby"
                        anchors.centerIn: parent
                }
        }

        property Camera cam: null
        property bool controlsVisible: cam.running && !standby.visible

        anchors.fill: parent

        property alias previewAnimationRunning: preview.animationRunning

        function setPreview(image) {
                preview.setPreview(image);
        }

        ModeButton {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.bottomMargin: 20
                visible: controlsVisible
        }

        PreviewImage {
                id: preview
        }

        ZoomSlider {
                id: zoom
                camera: cam
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.horizontalCenter: parent.horizontalCenter
                visible: controlsVisible
        }
}
