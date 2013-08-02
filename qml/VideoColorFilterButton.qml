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

    iconId: Data.cfIcon(settings.videoColorFilter)

    property CameraToolBarTools tools: CameraToolBarTools {
        CameraLabel {
            height: parent ? parent.height : 0
            text: qsTr("Filter")
            verticalAlignment: Text.AlignVCenter
        }

        CheckButton {
            iconId: Data.cfIcon(ColorTone.Normal)
            onClicked: settings.videoColorFilter = ColorTone.Normal
            checked: settings.videoColorFilter == ColorTone.Normal
        }

        CheckButton {
            iconId: Data.cfIcon(ColorTone.GrayScale)
            onClicked: settings.videoColorFilter = ColorTone.GrayScale
            checked: settings.videoColorFilter == ColorTone.GrayScale
        }

        CheckButton {
            iconId: Data.cfIcon(ColorTone.Sepia)
            onClicked: settings.videoColorFilter = ColorTone.Sepia
            checked: settings.videoColorFilter == ColorTone.Sepia
        }

        CheckButton {
            iconId: Data.cfIcon(ColorTone.Vivid)
            onClicked: settings.videoColorFilter = ColorTone.Vivid
            checked: settings.videoColorFilter == ColorTone.Vivid
        }

        CheckButton {
            iconId: Data.cfIcon(ColorTone.Negative)
            onClicked: settings.videoColorFilter = ColorTone.Negative
            checked: settings.videoColorFilter == ColorTone.Negative
        }

        CheckButton {
            iconId: Data.cfIcon(ColorTone.Solarize)
            onClicked: settings.videoColorFilter = ColorTone.Solarize
            checked: settings.videoColorFilter == ColorTone.Solarize
        }
    }
}
