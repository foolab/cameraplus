// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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

Button {
        id: button
        property string normalIcon: ""
        property string checkedIcon: ""
        property QtObject fader: null
        property int value: -1
        property bool doClose: true
        property int savedValue: -1

        width: visible ? 56 : 0
        height: visible ? 56 : 0
        iconSource: !visible ? "" : savedValue == value ? checkedIcon : normalIcon

        onClicked: {
                if (doClose) {
                        close();
                }
        }
}
