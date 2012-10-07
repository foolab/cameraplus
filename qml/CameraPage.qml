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

        property alias standbyWidget: standby

        property bool needsPipeline: true
        property int policyMode: CameraResources.None

        signal batteryLow

        Component.onCompleted: {
                if (Qt.application.active && needsPipeline) {
                        resourcePolicy.acquire(page.policyMode);
                }
        }

        onStatusChanged: {
                if (Qt.application.active && status == PageStatus.Activating) {
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
                if (Qt.application.active) {
                        resourcePolicy.acquire(page.policyMode);
                }
        }

        function handlePipeline() {
                if (!Qt.application.active) {
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
                target: Qt.application
                onActiveChanged: {
                        if (!Qt.application.active) {
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
                        if (cam.idle && !Qt.application.active) {
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
                // TODO: fade out transition
                id: standby
                color: "black"
                anchors.fill: parent
                visible: !Qt.application.active || !cam.running || !resourcePolicy.acquired
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
