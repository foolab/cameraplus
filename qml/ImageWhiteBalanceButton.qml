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
import "data.js" as Data

CameraToolIcon {
    id: button

    iconId: Data.wbIcon(settings.imageWhiteBalance)

    property CameraToolBarTools tools: CameraToolBarTools {
        CameraLabel {
            height: parent ? parent.height : 0
            text: qsTr("WB")
            verticalAlignment: Text.AlignVCenter
        }

        CheckButton {
            iconId: Data.wbIcon(WhiteBalance.Auto)
            onClicked: settings.imageWhiteBalance = WhiteBalance.Auto
            checked: settings.imageWhiteBalance == WhiteBalance.Auto
        }

        CheckButton {
            iconId: Data.wbIcon(WhiteBalance.Sunset)
            onClicked: settings.imageWhiteBalance = WhiteBalance.Sunset
            checked: settings.imageWhiteBalance == WhiteBalance.Sunset
        }

        CheckButton {
            iconId: Data.wbIcon(WhiteBalance.Cloudy)
            onClicked: settings.imageWhiteBalance = WhiteBalance.Cloudy
            checked: settings.imageWhiteBalance == WhiteBalance.Cloudy
        }

        CheckButton {
            iconId: Data.wbIcon(WhiteBalance.Flourescent)
            onClicked: settings.imageWhiteBalance = WhiteBalance.Flourescent
            checked: settings.imageWhiteBalance == WhiteBalance.Flourescent
        }

        CheckButton {
            iconId: Data.wbIcon(WhiteBalance.Tungsten)
            onClicked: settings.imageWhiteBalance = WhiteBalance.Tungsten
            checked: settings.imageWhiteBalance == WhiteBalance.Tungsten
        }
    }
}
