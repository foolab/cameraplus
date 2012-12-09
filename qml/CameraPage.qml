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

        function updatePolicy() {
                if (!resourcePolicy.acquire(page.policyMode)) {
                        cam.stop(force);
                }
                else {
                        handlePipeline();
                }
        }

        Component.onCompleted: {
                if (Qt.application.active && needsPipeline) {
                        updatePolicy();
                }
        }

        onStatusChanged: {
                if (Qt.application.active && status == PageStatus.Activating) {
                        updatePolicy();
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
                        updatePolicy();
                }
        }

        function handlePipeline() {
                var acquired = resourcePolicy.acquired;
                if (!acquired) {
                        cam.stop(true);
                        showError(qsTr("Resources lost. Stopping camera."));
                }
                else if (!Qt.application.active && !acquired) {
                        cam.stop(true);
                        showError(qsTr("Resources lost. Stopping camera."));
                }
                else if (!Qt.application.active) {
                        cam.stop();
                }
                else if (acquired && page.needsPipeline && !cam.running) {
                        if (!cam.start()) {
                                showError(qsTr("Failed to start camera. Please restart the application."));
                        }
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
                                updatePolicy();
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
                // TODO: there is a toolbar visible on the first startup
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
