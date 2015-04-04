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

Column {
    property string currentAspectRatio:
        imageSettings.aspectRatioForResolution(deviceSettings().imageResolution);
    spacing: 10

    Binding {
        target: imageSettings.resolutions
        property: "aspectRatio"
        value: currentAspectRatio
    }

    SectionHeader {
        text: qsTr("Aspect ratio")
        visible: aspectRatioRow.visible
    }

    Row {
        id: aspectRatioRow
        width: parent.width
        enabled: camera ? camera.idle : false
        visible: imageSettings.aspectRatioCount > 1

        Repeater {
            id: aspectRatios
            model: imageSettings.aspectRatios
            delegate: CameraCheckButton {
                width: aspectRatioRow.width / aspectRatios.count
                text: qsTr(modelData)
                checked: currentAspectRatio == modelData
                onClicked: {
                    deviceSettings().imageResolution = imageSettings.bestResolution(modelData, deviceSettings().imageResolution)
                    imageSettings.setImageResolution()
                }
            }
        }
    }

    SectionHeader {
        text: qsTr("Resolution")
        visible: resolutionsRow.visible
    }

    Flow {
        id: resolutionsRow
        width: parent.width
        enabled: camera ? camera.idle : false
        visible: imageSettings.resolutions.count > 1

        Repeater {
            id: resolutions
            model: imageSettings.resolutions.aspectRatio == currentAspectRatio ?
                imageSettings.resolutions : undefined

            delegate: CameraCheckButton {
                width: resolutionsRow.width / Math.min(resolutions.count, 5)
                text: qsTr("%1 MP").arg(megaPixels)
                checked: deviceSettings().imageResolution == resolutionId
                onClicked: {
                    deviceSettings().imageResolution = resolutionId
                    imageSettings.setImageResolution()
                }
            }
        }
    }
}
