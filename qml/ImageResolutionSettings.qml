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

@IMPORT_QT_QUICK@

Column {
    spacing: 10

    SectionHeader {
        text: qsTr("Aspect ratio")
    }

    CameraButtonRow {
        id: aspectRatioRow
        width: parent.width
        enabled: cam.idle
        exclusive: false

        Repeater {
            model: imageSettings.aspectRatios
            delegate: CameraButton {
                text: qsTr(modelData)
                checked: settings.imageAspectRatio == modelData
                onClicked: settings.imageAspectRatio = modelData
            }
        }
    }

    SectionHeader {
        text: qsTr("Resolution")
    }

    CameraButtonRow {
        id: resolutionsRow
        width: parent.width
        enabled: cam.idle
        exclusive: false

        Binding {
            target: imageSettings.resolutions
            property: "aspectRatio"
            value: settings.imageAspectRatio
        }

        Repeater {
            id: resolutions
            model: imageSettings.resolutions.aspectRatio == settings.imageAspectRatio ?
                imageSettings.resolutions : undefined

            delegate: CameraButton {
                font.capitalization: Font.Capitalize
                text: qsTr("%1 %2 Mpx").arg(resolutionName).arg(megaPixels)
                checked: settings.imageResolution == resolutionName
                onClicked: settings.imageResolution = resolutionName
            }
        }
    }
}
