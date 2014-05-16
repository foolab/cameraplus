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

CameraToolBarTools {
    property list<ToolsModelItem> toolsModel: [
        ToolsModelItem {icon: cameraTheme.colorFilterNormalIconId; value: ColorTone.Normal; label: qsTr("Normal"); visible: deviceFeatures().isNormalVideoColorToneSupported},
        ToolsModelItem {icon: cameraTheme.colorFilterGrayScaleIconId; value: ColorTone.GrayScale; label: qsTr("Gray scale"); visible: deviceFeatures().isGrayScaleVideoColorToneSupported},
        ToolsModelItem {icon: cameraTheme.colorFilterSepiaIconId; value: ColorTone.Sepia; label: qsTr("Sepia"); visible: deviceFeatures().isSepiaVideoColorToneSupported},
        ToolsModelItem {icon: cameraTheme.colorFilterVividIconId; value: ColorTone.Vivid; label: qsTr("Vivid"); visible: deviceFeatures().isVividVideoColorToneSupported},
        ToolsModelItem {icon: cameraTheme.colorFilterNegativeIconId; value: ColorTone.Negative; label: qsTr("Negative"); visible: deviceFeatures().isNegativeVideoColorToneSupported},
        ToolsModelItem {icon: cameraTheme.colorFilterSolarizeIconId; value: ColorTone.Solarize; label: qsTr("Solarize"); visible: deviceFeatures().isSolarizeVideoColorToneSupported}
    ]

    CameraLabel {
        height: parent.height
        text: qsTr("Filter")
        verticalAlignment: Text.AlignVCenter
    }

    Repeater {
        model: toolsModel

        delegate: CheckButton {
            iconSource: modelData.icon
            onClicked: deviceSettings().videoColorFilter = value
            checked: deviceSettings().videoColorFilter == value
            visible: modelData.visible
            onCheckedChanged: {
                if (checked) {
                    selectedLabel.text = label
                }
            }
        }
    }
}
