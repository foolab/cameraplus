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
        ToolsModelItem {icon: cameraTheme.isoAudoIconId; value: 0; visible: deviceFeatures().is0IsoModeSupported},
        ToolsModelItem {icon: cameraTheme.iso100IconId; value: 100; visible: deviceFeatures().is100IsoModeSupported},
        ToolsModelItem {icon: cameraTheme.iso200IconId; value: 200; visible: deviceFeatures().is200IsoModeSupported},
        ToolsModelItem {icon: cameraTheme.iso400IconId; value: 400; visible: deviceFeatures().is400IsoModeSupported},
        ToolsModelItem {icon: cameraTheme.iso800IconId; value: 800; visible: deviceFeatures().is800IsoModeSupported}
    ]

    CameraLabel {
        height: parent.height
        text: qsTr("ISO")
        verticalAlignment: Text.AlignVCenter
    }

    Repeater {
        model: toolsModel
        delegate: CheckButton {
            iconSource: modelData.icon
            onClicked: deviceSettings().imageIso = value
            checked: deviceSettings().imageIso == value
            visible: modelData.visible
        }
    }
}
