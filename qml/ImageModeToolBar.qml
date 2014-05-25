// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2014 Mohammed Sameer <msameer@foolab.org>
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

import QtQuick 2.0
import QtCamera 1.0

CameraToolBar {
    id: toolBar
    property CameraLabel selectedLabel
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 20
    anchors.left: parent.left
    anchors.leftMargin: 20
    opacity: 0.5
    targetWidth: parent.width - (anchors.leftMargin * 2)
    expanded: settings.showToolBar
    onExpandedChanged: settings.showToolBar = expanded

    tools: CameraToolBarTools {
        CameraToolIcon {
            iconSource: cameraTheme.flashIcon(deviceSettings().imageFlashMode)
            onClicked: toolBar.push(Qt.resolvedUrl("FlashButton.qml"), {"selectedLabel": selectedLabel})
            visible: deviceFeatures().numberOfFlashModes > 1
        }

        CameraToolIcon {
            iconSource: cameraTheme.imageSceneModeIcon(deviceSettings().imageSceneMode)
            onClicked: toolBar.push(Qt.resolvedUrl("ImageSceneButton.qml"), {"selectedLabel": selectedLabel})
            visible: deviceFeatures().numberOfImageSceneModes > 1
        }

        CameraToolIcon {
            iconSource: deviceSettings().imageEvComp == 0 ? cameraTheme.cameraManualExposureIconId : ""
            onClicked: toolBar.push(Qt.resolvedUrl("ImageEvCompButton.qml"))

            CameraLabel {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                visible: deviceSettings().imageEvComp != 0
                text: deviceSettings().imageEvComp == 0 ? "" : deviceSettings().imageEvComp.toFixed(1)
            }
        }

        CameraToolIcon {
            iconSource: cameraTheme.whiteBalanceIcon(deviceSettings().imageWhiteBalance)
            onClicked: toolBar.push(Qt.resolvedUrl("ImageWhiteBalanceButton.qml"), {"selectedLabel": selectedLabel})
            visible: deviceFeatures().numberOfImageWhiteBalanceModes > 1
        }

        CameraToolIcon {
            iconSource: cameraTheme.colorFilterIcon(deviceSettings().imageColorFilter)
            onClicked: toolBar.push(Qt.resolvedUrl("ImageColorFilterButton.qml"), {"selectedLabel": selectedLabel})
            visible: deviceFeatures().numberOfImageColorTones > 1
        }

        CameraToolIcon {
            iconSource: cameraTheme.isoIcon(deviceSettings().imageIso)
            onClicked: toolBar.push(Qt.resolvedUrl("ImageIsoButton.qml"))
            visible: deviceFeatures().numberOfIsoModes > 1
        }

        DeviceSelector {
            enabled: camera.idle
            visible: activePlugin.primaryCameraSupported && activePlugin.secondaryCameraSupported
        }
    }
}
