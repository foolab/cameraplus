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

    property string __aspectRatio: settings.device == 1 ?
        settings.secondaryImageAspectRatio : settings.primaryImageAspectRatio
    property string __resolution: settings.device == 1 ?
        settings.secondaryImageResolution : settings.primaryImageResolution

    SectionHeader {
        text: qsTr("Aspect ratio")
        visible: aspectRatioRow.visible
    }

    CameraButtonRow {
        id: aspectRatioRow
        width: parent.width
        enabled: camera ? camera.idle : false
        exclusive: false
        visible: imageSettings.aspectRatioCount > 1

        Repeater {
            model: imageSettings.aspectRatios
            delegate: CameraButton {
                text: qsTr(modelData)
                checked: __aspectRatio == modelData
                onClicked: {
                    if (settings.device == 1) {
                        settings.secondaryImageAspectRatio = modelData
                    } else {
                        settings.primaryImageAspectRatio = modelData
                    }
                }
            }
        }
    }

    SectionHeader {
        text: qsTr("Resolution")
        visible: resolutionsRow.visible
    }

    CameraButtonRow {
        id: resolutionsRow
        width: parent.width
        enabled: camera ? camera.idle : false
        exclusive: false
        visible: imageSettings.resolutions.count > 1

        Binding {
            target: imageSettings.resolutions
            property: "aspectRatio"
            value: settings.primaryImageAspectRatio
            when: settings.device == 0
        }

        Binding {
            target: imageSettings.resolutions
            property: "aspectRatio"
            value: settings.secondaryImageAspectRatio
            when: settings.device == 1
        }

        Repeater {
            id: resolutions

            model: imageSettings.resolutions.aspectRatio == __aspectRatio ?
                imageSettings.resolutions : undefined

            delegate: CameraButton {
                capitalize: true
                text: qsTr("%1 %2 Mpx").arg(resolutionName).arg(megaPixels)
                checked: __resolution == resolutionName
                onClicked: {
                    if (settings.device == 1) {
                        settings.secondaryImageResolution = resolutionName
                    } else {
                        settings.primaryImageResolution = resolutionName
                    }
                }
            }
        }
    }
}
