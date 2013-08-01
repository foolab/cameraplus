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

    iconId: Data.ismIcon(settings.imageSceneMode)

    property list<Item> items: [
        CameraLabel {
            height: parent ? parent.height : 0
            text: qsTr("Scene")
            verticalAlignment: Text.AlignVCenter
        },
        CheckButton {
            iconId: Data.ismIcon(Scene.Auto)
            onClicked: settings.imageSceneMode = Scene.Auto
            checked: settings.imageSceneMode == Scene.Auto
        },
        CheckButton {
            iconId: Data.ismIcon(Scene.Closeup)
            onClicked: settings.imageSceneMode = Scene.Closeup
            checked: settings.imageSceneMode == Scene.Closeup
        },
        CheckButton {
            iconId: Data.ismIcon(Scene.Landscape)
            onClicked: settings.imageSceneMode = Scene.Landscape
            checked: settings.imageSceneMode == Scene.Landscape
        },
        CheckButton {
            iconId: Data.ismIcon(Scene.Portrait)
            onClicked: settings.imageSceneMode = Scene.Portrait
            checked: settings.imageSceneMode == Scene.Portrait
        },
        CheckButton {
            iconId: Data.ismIcon(Scene.Night)
            onClicked: settings.imageSceneMode = Scene.Night
            checked: settings.imageSceneMode == Scene.Night
        },
        CheckButton {
            iconId: Data.ismIcon(Scene.Sport)
            onClicked: settings.imageSceneMode = Scene.Sport
            checked: settings.imageSceneMode == Scene.Sport
        }
    ]
}
