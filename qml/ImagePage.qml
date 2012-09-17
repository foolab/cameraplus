// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0
import CameraPlus 1.0
import "data.js" as Data

CameraPage {
        id: page

        policyMode: CameraResources.Image
        controlsVisible: capture.visible && cam.running && !standbyWidget.visible

        Button {
                id: capture
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                iconSource: "image://theme/icon-m-camera-shutter"
                width: 75
                height: 75
                opacity: 0.5
                onClicked: {
                        if (!fileSystem.available) {
                                showError(qsTr("Camera cannot capture images in mass storage mode."));
                                return;
                        }

                        metaData.setMetaData();

                        if (!imageMode.capture(fileNaming.imageFileName())) {
                                showError(qsTr("Failed to capture image. Please restart the camera."));
                        }
                }

                visible: imageMode.canCapture && !cameraMode.animationRunning && !previewAnimationRunning && cam.running
        }

        ImageMode {
                id: imageMode
                camera: cam
                onPreviewAvailable: page.setPreview(preview);
        }

        FlashButton {
                id: flash
                visible: controlsVisible
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 20
                anchors.leftMargin: 20
        }

        ImageSceneButton {
                id: scene
                visible: controlsVisible
                anchors.top: flash.bottom
                anchors.left: parent.left
                anchors.topMargin: 10
                anchors.leftMargin: 20
        }

        EvCompButton {
                id: evComp
                visible: controlsVisible
                anchors.top: scene.bottom
                anchors.left: parent.left
                anchors.topMargin: 10
                anchors.leftMargin: 20
        }

        Indicators {
                visible: controlsVisible
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 20

                onClicked: openFile("ImageSettingsPage.qml");

                Indicator {
                        id: resolutionIndicator
                        // TODO:
                }

                Indicator {
                        id: wbIndicator
                        anchors.top: resolutionIndicator.bottom
                        source: "image://theme/" + Data.wbIcon(settings.imageWhiteBalance) + "-screen"
                        visible: settings.imageWhiteBalance != WhiteBalance.Auto
                }

                Indicator {
                        id: cfIndicator
                        anchors.top: wbIndicator.bottom
                        source: "image://theme/" + Data.cfIcon(settings.imageColorFilter) + "-screen"
                        visible: settings.imageColorFilter != ColorTone.Normal
                }

                Indicator {
                        id: isoIndicator
                        visible: settings.imageIso != 0
                        source: "image://theme/" + Data.isoIcon(settings.imageIso);
                        anchors.top: cfIndicator.bottom
                }

                Indicator {
                        id: gpsIndicator
                        anchors.top: isoIndicator.bottom
                        visible: settings.useGps
                        source: "image://theme/icon-m-camera-location"

                        PropertyAnimation on opacity  {
                                easing.type: Easing.OutSine
                                loops: Animation.Infinite
                                from: 0.2
                                to: 1.0
                                duration: 1000
                                running: settings.useGps && !positionSource.position.longitudeValid
                                alwaysRunToEnd: true
                        }
                }
        }

        Button {
                id: cameraRoll
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.topMargin: 20
                anchors.rightMargin: 20
                width: 56
                height: 56

                opacity: 0.5
                iconSource: "image://theme/icon-m-camera-roll"
                onClicked: openFile("PostCapturePage.qml");
                visible: controlsVisible
        }
}
