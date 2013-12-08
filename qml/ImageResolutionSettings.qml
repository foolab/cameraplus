// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
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
    property Camera camera: null

    spacing: 10

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
            delegate: CameraButton {
                width: aspectRatioRow.width / aspectRatios.count
                text: qsTr(modelData)
                checked: deviceSettings().imageAspectRatio == modelData
                onClicked: deviceSettings().imageAspectRatio = modelData
            }
        }
    }

    SectionHeader {
        text: qsTr("Resolution")
        visible: resolutionsRow.visible
    }

    Row {
        id: resolutionsRow
        width: parent.width
        enabled: camera ? camera.idle : false
        visible: imageSettings.resolutions.count > 1

        Binding {
            target: imageSettings.resolutions
            property: "aspectRatio"
            value: deviceSettings().imageAspectRatio
        }

        Repeater {
            id: resolutions
            model: imageSettings.resolutions.aspectRatio == deviceSettings().imageAspectRatio ?
                imageSettings.resolutions : undefined

            delegate: CameraButton {
                width: resolutionsRow.width / resolutions.count
                capitalize: true
                text: qsTr("%1 %2 Mpx").arg(resolutionName).arg(megaPixels)
                checked: deviceSettings().imageResolution == resolutionName
                onClicked: deviceSettings().imageResolution = resolutionName
            }
        }
    }
}
