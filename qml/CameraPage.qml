// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0
import CameraPlus 1.0
import QtMobility.systeminfo 1.2

Page {
        id: page

        property bool needsPipeline: true
        property int policyMode: CameraResources.None

        property Camera cam: null
        property bool controlsVisible: cam.running && !standby.visible
        property bool zoomVisible: true
        property bool modesVisible: true
        property bool standbyVisible: true
        property bool focusReticleVisible: true

        property alias previewAnimationRunning: preview.animationRunning

        signal batteryLow

        function policyLost() {
                // Nothing
        }

        onStatusChanged: {
                if (status == PageStatus.Active) {
                        focusReticle.setRegionOfInterest();
                }
        }

        Rectangle {
                // TODO: fade out transition
                // TODO: there is a toolbar visible on the first startup
                id: standby
                color: "black"
                anchors.fill: parent
                visible: standbyVisible && page.status == PageStatus.Active && pipelineManager.showStandBy
                z: 1
                Image {
                        id: icon
                        source: "image://theme/icon-l-camera-standby"
                        anchors.centerIn: parent
                }

                Label {
                        anchors.top: icon.bottom
                        anchors.right: parent.right
                        anchors.left: parent.left
                        text: qsTr("Resources lost")
                        color: "white"
                        font.pixelSize: 36
                        horizontalAlignment: Text.AlignHCenter
                        visible: pipelineManager.state == "policyLost"
                }
        }

        function setPreview(image) {
                preview.setPreview(image);
        }

        ModeButton {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.bottomMargin: 20
                visible: controlsVisible && modesVisible
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
                visible: controlsVisible && zoomVisible
        }

        function checkDiskSpace() {
                return fileSystem.hasFreeSpace(fileNaming.path);
        }

        function checkBattery() {
                // We are fine if we are connected to the charger:
                if (batteryMonitor.chargingState == BatteryInfo.Charging) {
                        return true;
                }

                // If we have enough battery then we are fine:
                if (batteryMonitor.batteryStatus > BatteryInfo.BatteryCritical) {
                        return true;
                }

                return false;
        }

        BatteryInfo {
                id: batteryMonitor
                monitorChargingStateChanges: cam.running
                monitorBatteryStatusChanges: cam.running

                onChargingStateChanged: {
                        if (!checkBattery()) {
                                parent.batteryLow();
                        }
                }

                onBatteryStatusChanged:  {
                        if (!checkBattery()) {
                                parent.batteryLow();
                        }
                }
        }
}
