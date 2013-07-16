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

import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0
import "data.js" as Data

ToolIcon {
    id: button
    iconSource: "image://theme/" + Data.cfIcon(settings.imageColorFilter)

    property list<Item> items: [
        Label {
            height: parent ? parent.height : 0
            text: qsTr("Filter")
            verticalAlignment: Text.AlignVCenter
        },
        CheckButton {
            normalIcon: "image://theme/" + Data.cfIcon(value)
            checkedIcon: "image://theme/" + Data.cfSelectedIcon(value)
            value: ColorTone.Normal
            savedValue: settings.imageColorFilter
            onClicked: settings.imageColorFilter = value
        },
        CheckButton {
            normalIcon: "image://theme/" + Data.cfIcon(value)
            checkedIcon: "image://theme/" + Data.cfSelectedIcon(value)
            value: ColorTone.GrayScale
            savedValue: settings.imageColorFilter
            onClicked: settings.imageColorFilter = value
        },
        CheckButton {
            normalIcon: "image://theme/" + Data.cfIcon(value)
            checkedIcon: "image://theme/" + Data.cfSelectedIcon(value)
            value: ColorTone.Sepia
            savedValue: settings.imageColorFilter
            onClicked: settings.imageColorFilter = value
        },
        CheckButton {
            normalIcon: "image://theme/" + Data.cfIcon(value)
            checkedIcon: "image://theme/" + Data.cfSelectedIcon(value)
            value: ColorTone.Vivid
            savedValue: settings.imageColorFilter
            onClicked: settings.imageColorFilter = value
        },
        CheckButton {
            normalIcon: "image://theme/" + Data.cfIcon(value)
            checkedIcon: "image://theme/" + Data.cfSelectedIcon(value)
            value: ColorTone.Negative
            savedValue: settings.imageColorFilter
            onClicked: settings.imageColorFilter = value
        },
        CheckButton {
            normalIcon: "image://theme/" + Data.cfIcon(value)
            checkedIcon: "image://theme/" + Data.cfSelectedIcon(value)
            value: ColorTone.Solarize
            savedValue: settings.imageColorFilter
            onClicked: settings.imageColorFilter = value
        }
    ]
}
