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

import QtQuick 1.1
import com.nokia.meego 1.1
import CameraPlus 1.0

Item {
    id: window
    width: screen.displayWidth
    height: screen.displayHeight

    property Component initialPage
    property Item page
    property url standByIcon
    property bool active: platformQuirks.windowActive

    PlatformQuirks {
        id: platformQuirks
    }

    Binding {
        target: window
        property: "rotation"
        value: screen.rotation
    }

    Behavior on rotation {
        PropertyAnimation { duration: 200 }
    }

    Component.onDestruction: page.destroy()
    Component.onCompleted: {
        screen.setAllowedOrientations(Screen.Landscape | Screen.LandscapeInverted)
        theme.inverted = true

        if (initialPage) {
            page = initialPage.createObject(window)
            page.visible = true
        }
    }

    function popAll() {
        // Nothing
    }
}
